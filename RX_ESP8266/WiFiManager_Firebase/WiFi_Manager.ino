#define EEPROM_SIZE 96  //Ukuran EEPROM
#define SSID_ADDR 0     //Alamat SSID
#define PASS_ADDR 32    //Alamat Password

void WiFiManagerSetup(){
  EEPROM.begin(EEPROM_SIZE);
  WiFi.mode(WIFI_AP_STA);
  // Baca kredensial WiFi dari EEPROM
  
  String storedSSID, storedPass;
  readWiFiCredentials(storedSSID, storedPass);
  // Coba konek ke WiFi tersimpan
  if (connectToWiFi(storedSSID, storedPass)) {
  connectedSSID = storedSSID;
  }
  WiFi.softAP(ID_ESP);
  Serial.print("📡 SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  // Web handler
  server.on("/", handleRoot);
  server.on("/setwifi", HTTP_POST, handleSetWiFi);
  server.begin();

}

//Fungsi untuk menyimpan data SSID dan Password ke EEPROM
void saveWiFiCredentials(const String& ssid, const String& password) {
  for (int i = 0; i < 32; ++i) {
    EEPROM.write(SSID_ADDR + i, i < ssid.length() ? ssid[i] : 0);
    EEPROM.write(PASS_ADDR + i, i < password.length() ? password[i] : 0);
  }
  EEPROM.commit();
  Serial.println("✅ WiFi credentials saved to EEPROM.");
}

//Fungsi untuk membaca data SSID dan Password dari EEPROM
void readWiFiCredentials(String& ssid, String& password) {
  char ssidBuf[33], passBuf[33];
  for (int i = 0; i < 32; ++i) {
    ssidBuf[i] = EEPROM.read(SSID_ADDR + i);
    passBuf[i] = EEPROM.read(PASS_ADDR + i);
  }
  ssidBuf[32] = 0;
  passBuf[32] = 0;

  ssid = String(ssidBuf).c_str();
  password = String(passBuf).c_str();
}

//Fungsi untuk menghubungkan ESP ke SSID dan Paswsword yang telah didapatkan dari EEPROM
bool connectToWiFi(const String& ssid, const String& password) {
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("🔄 Connecting to saved WiFi: ");
  Serial.print(ssid); Serial.print(" "); Serial.println(password);
  
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 20) {
    delay(500);
    Serial.print(".");
    tries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Connected to WiFi!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("\n❌ Failed to connect to WiFi.");
    return false;
  }
}


void handleRoot() {
  server.send(200, "text/html",
    "<form method='POST' action='/setwifi'>"
    "SSID: <input name='ssid'><br>"
    "Password: <input name='password'><br>"
    "<input type='submit'></form>");
}

void handleSetWiFi() {
  String ssid = server.arg("ssid");
  String password = server.arg("password");

  server.send(200, "text/plain", "📶 Trying to connect to " + ssid);
  
  saveWiFiCredentials(ssid, password);
  if (connectToWiFi(ssid, password)) {
  connectedSSID = ssid;
  }
}
