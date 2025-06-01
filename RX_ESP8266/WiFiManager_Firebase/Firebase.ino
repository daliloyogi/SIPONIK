int jumlahChild = 0;  //Variabel untuk menyimpan data

//Fungsi untuk SetUp Konfigurasi yang diperlukan untuk akses data ke Firebase
void FirebaseSetup() {
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = FIREBASE_HOST;
  // config.token_status_callback = tokenStatusCallback;
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.begin(&config, &auth);
}

//Fungsi untuk upload SATU data ke Firebase (Bisa tipe data Int, Float, Bool, dan String)
template<typename T>
bool setFirebaseData(FirebaseData &fbdo, const String &path, T value) {
  bool success = false;

  if constexpr (std::is_same<T, int>::value) {
    success = Firebase.setInt(fbdo, path, value);
  } else if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value) {
    success = Firebase.setFloat(fbdo, path, value);
  } else if constexpr (std::is_same<T, bool>::value) {
    success = Firebase.setBool(fbdo, path, value);
  } else if constexpr (std::is_same<T, String>::value || std::is_same<T, const char *>::value) {
    success = Firebase.setString(fbdo, path, value);
  } else {
    Serial.println("Unsupported data type for Firebase");
    return false;
  }

  if (success) {
    // Serial.println("Data uploaded successfully!");
  } else {
    Serial.println("Error uploading data: " + fbdo.errorReason());
  }

  return success;
}

//Fungsi untuk upload BEBERAPA data sekaligus, data yang dikirim harus dalam type data FLOAT
bool updateFirebaseJSON(FirebaseData &fbdo, const String &path, std::initializer_list<std::pair<const char *, float>> data) {
  FirebaseJson json;
  for (auto &item : data) {
    json.set(item.first, item.second);
  }

  if (Firebase.updateNode(fbdo, path, json)) {
    // Serial.println("JSON data uploaded successfully!");
    return true;
  } else {
    Serial.println("Error uploading JSON: " + fbdo.errorReason());
    return false;
  }
}

bool setNotification(FirebaseData &fbdo, const String &path, std::initializer_list<std::pair<const char *, String>> data) {
  if (Firebase.getShallowData(fbdo, path)) {
    String jsonStr = fbdo.stringData();
    jumlahChild = 0;
    for (int i = 0; i < jsonStr.length(); i++) {
      if (jsonStr[i] == ':') jumlahChild++;  // Hitung jumlah child berdasarkan karakter ':', ini adalah jumlah data kita
    }
  } else {
    Serial.print("Error: ");
    Serial.println(fbdo.errorReason());
  }

  FirebaseJson JSON;
  for (auto &item : data) {
    JSON.set(item.first, item.second);
  }

  String notifPath = path + "/" + String(jumlahChild);
  if (Firebase.updateNode(fbdo, notifPath, JSON)) {
    // Serial.println("JSON data uploaded successfully!");
    setFirebaseData(fbdo, notifPath + "/isRead", false);
    return true;
  } else {
    Serial.println("Error uploading JSON: " + fbdo.errorReason());
    return false;
  }
}
