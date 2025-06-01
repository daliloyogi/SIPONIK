#include <ESP8266WiFi.h>       //Library WiFi untuk ESP8266
#include <ESP8266WebServer.h>  //Library untuk berkomunikasi lewat Server
#include <FirebaseESP8266.h>   //Library untuk set dan get data Firebase
#include <EEPROM.h>            //Library untuk menyimpan SSID dan Password WiFi ke EEPROM

const char* ID_ESP = "SIPONIK7";  //Nama ID ESP yang sudah ditentukan

//Dependensi Firebase diambill dari dasboard Database Firebase
#define FIREBASE_HOST "pengabdianunnes-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyB4yMjX_ojFJ85gF8ulwcwnWueFU668EO0"

//Email dan Password yang terdaftar di Database untuk autentikasi akses data ke Database
#define USER_EMAIL "farmsandibuana@gmail.com"
#define USER_PASSWORD "f4rms4nd1bu4n4"

//Object Firebase yang dipakai
FirebaseData fbdo;      //Untuk urusan upload dan download data
FirebaseAuth auth;      //Untuk urusan autentikasi / perizinan akses data
FirebaseConfig config;  //Untuk konfigurasi data dan autentikasi

//Objek server
ESP8266WebServer server(80);

//Variabel untuk menyimpan  data SSID yang terhubung kemudian di upload ke Database
String connectedSSID;

//Inisiasi timer untuk menghindari penggunaan Delay()
unsigned long prevT, currT, interval = 5000;

//Flag untuk mengetahui konektivitas dari firebase apakah terhubung atau tidak
bool firebaseInitialized = false;

//Variabel string untuk menyimpan data serial
String incomingData = "";
float temperature = 0.0;
float ambient = 0.0;
float pH = 0.0;
int nutrisi = 0;
int waterLevel = 0;

void setup() {
  Serial.begin(115200);
  WiFiManagerSetup();  //Fungsi WiFiManagerSetup() dapat dilihat pada file WiFi_Manager.ino
}

void loop() {
  server.handleClient();  //Untuk menghandle proses pergantian SSID dan Password WiFi

  //Pembacaan data Serial
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      // End of message reached, parse it
      parseData(incomingData);
      incomingData = "";  // Clear buffer for next message
    } else {
      incomingData += c;  // Accumulate characters
    }
  }

  //Jika Wifi sudah terhubung namun firebase belum terhubung, maka:
  if (WiFi.status() == WL_CONNECTED && !firebaseInitialized) {
    FirebaseSetup();             //Menghubungkan ke Firebase
    firebaseInitialized = true;  //Flag Firebase diubah ke true
  }

  //Jika firebase belum siap, maka:
  if (!firebaseInitialized) {
    yield();
    return;  //Skip,
  }

  //Path atau Directory penyimpanan data
  String basePath = "Siponik/" + String(ID_ESP) + "/Data";

  currT = millis();
  //Jika delay lebih dari sama dengan interval maka
  if ((currT - prevT) >= interval) {

    //Upload data connectedSSID ke database bisa mengakibatkan error jika SSID kosong, makanya perlu diperiksa
    //Jika panjang SSID lebih dari 0 (artinya jika SSIS sudah ada), maka:
    if (connectedSSID.length() > 0) {
      //Upload data connectedSSID ke firebase dengan lokasi di basePath + "/WifiSSID"
      setFirebaseData(fbdo, basePath + "/WifiSSID", connectedSSID);
    }

    //Upload beberapa data ke Firebase menggunakan format JSON
    //Data diatas lalu dibentuk dalam format json dibawah ini
    updateFirebaseJSON(fbdo, basePath, { { "SuhuRuang", ambient}, { "SuhuAir", temperature }, { "KetinggianAir", waterLevel }, { "Nutrisi", nutrisi }, { "pH", pH } });

    //Upload Notifikasi
    String notifikasiPath = "/Siponik/" + String(ID_ESP) + "/Notifikasi";
    String Pesan = "Ini adalah contoh pesan";
    String Judul = "Ini Judul";
    setNotification(fbdo, notifikasiPath, { { "title", Judul }, { "message", Pesan } });

    prevT = currT;
  }


  yield();  //Sama seperti delay(1) namun tidak menyebabkan blocking, bertujuan untuk menyediakan "nafas" untuk ESP8266
}
