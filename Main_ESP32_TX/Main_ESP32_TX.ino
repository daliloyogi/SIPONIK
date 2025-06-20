#include "DFRobot_PH.h"
#include <EEPROM.h>
#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <esp_now.h>

// UART1 ESP32
#define UART1_TX 17
#define UART1_RX 16

// AlOKASI PIN
#define PH_PIN 34
#define TDS_PIN 35
#define DS18B2_PIN 33
#define TRIG_PIN 25
#define ECHO_PIN 26
#define SELENOID 23

#define SOUND_SPEED 0.034

DFRobot_PH ph;
OneWire ds(DS18B2_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);  // I2C default address 0x27, or 0x3F

float voltage, pH;
float tdsVoltage = 0, Nutrisi = 0;
long duration;
float KetinggianAir, distanceCm;
float SuhuAir = 0.0;
float depthCm = 100.0;  // Ganti dengan kedalaman kolam (cm)
float suhuRuang = 0.0;

/*
variabel Sensor :
TDS         > Nutrisi
PH          > pH
Ultrasonic  > KetinggianAir
DS18B2      > SuhuAir
*/

byte Light[8] = {
  0b00011,
  0b00111,
  0b01110,
  0b11111,
  0b11111,
  0b00110,
  0b01100,
  0b11000
};

byte Degree[8] = {
  0b01100,
  0b10010,
  0b10010,
  0b01100,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

typedef struct struct_message {
  float temperature;
} struct_message;

struct_message incomingData;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, UART1_RX, UART1_TX);

  EEPROM.begin(512);
  analogReadResolution(12);
  ph.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, OUTPUT);
  pinMode(SELENOID, OUTPUT);
  digitalWrite(SELENOID, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  IntroDisplay();
}

void loop() {
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) {
    timepoint = millis();

    ultrasonic();  // Data Ketinggian Air (%)
    dallas();      // Data Suhu Air (Celcius)
    TDS();         // Data Nutrisi (ppm)
    PH();          // Data Index PH

    if (KetinggianAir <= 70) {
      digitalWrite(SELENOID, LOW);
      SelenoidOnDisplay();
    }

    // Format as comma-separated string (CSV)
    String data = String(pH, 1) + "," + String(Nutrisi) + "," + String(KetinggianAir) + "," + String(SuhuAir, 1) + "," + String(suhuRuang);
    Serial1.println(data);            // Send to ESP8266 via UART1
    Serial.println("Sent: " + data);  // Debug output

    // Output LCD
    MainDisplay();
  }

  // Kalibrasi pH jika diperlukan
  // ph.calibration(voltage, temperature);
}
