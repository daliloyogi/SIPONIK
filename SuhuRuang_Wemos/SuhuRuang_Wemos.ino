#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

// Replace with the MAC address of the ESP32 receiver
uint8_t broadcastAddress[] = {0x8c, 0x4f, 0x00, 0x29, 0x66, 0x50};

typedef struct struct_message {
  float temperature;
} struct_message;

struct_message bmeData;

void onSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Send status: ");
  Serial.println(sendStatus == 0 ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (!bme.begin(0x76)) {
    Serial.println("BME280 not found!");
    while (1);
  }

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed!");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(onSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  bmeData.temperature = bme.readTemperature();

  esp_now_send(broadcastAddress, (uint8_t *)&bmeData, sizeof(bmeData));

  Serial.printf("Sent -> Temp: %.2f", bmeData.temperature,);

  delay(1000);
}