// UART1 pin definitions for ESP32 (u can adjust as u need)
#define UART1_TX 17  // Connects to ESP8266 RX
#define UART1_RX 16  // Not used, unless you want two-way comms

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, UART1_RX, UART1_TX);  // UART1 to ESP8266
  Serial.println("ESP32 started - sending data to ESP8266...");
}

void loop() {
  // Simulate sensor values
  float pH = random(50, 80) / 10.0;             // Range: 5.0 - 8.0
  int tdsValue = random(100, 1000);             // Range: 100 - 1000 ppm
  int waterLevel = random(70, 100);             // Range: 0 - 100 percent
  float temperature = random(200, 350) / 10.0;  // Range: 20.0 - 35.0 °C
  float ambient = random(200, 350) / 10.0;      // Range: 20.0 - 35.0 °C

  // Format as comma-separated string (CSV)
  String data = String(pH, 1) + "," + String(tdsValue) + "," + String(waterLevel) + "," + String(temperature, 1) + "," + String(ambient, 1);

  // Send to ESP8266 via UART1
  Serial1.println(data);            // Send via UART1
  Serial.println("Sent: " + data);  // Debug output

  delay(1000);  // Send once every second
}
