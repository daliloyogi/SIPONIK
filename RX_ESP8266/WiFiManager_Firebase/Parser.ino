void parseData(const String& data) {

  int index1 = data.indexOf(',');
  int index2 = data.indexOf(',', index1 + 1);
  int index3 = data.indexOf(',', index2 + 1);
  int index4 = data.indexOf(',', index3 + 1);

  if (index1 > 0 && index2 > index1 && index3 > index2) {
    // Extract and convert
    pH          = data.substring(0, index1).toFloat();
    nutrisi     = data.substring(index1 + 1, index2).toInt();
    waterLevel  = data.substring(index2 + 1, index3).toInt();
    temperature = data.substring(index3 + 1).toFloat();
    ambient     = data.substring(index4 + 1).toFloat();

    Serial.println("Data Received  : " + data);
    Serial.println("Parsing Result : ");
    Serial.print("  index pH    : "); Serial.println(pH);
    Serial.print("  nutrisi     : "); Serial.println(nutrisi);
    Serial.print("  waterLevel  : "); Serial.println(waterLevel);
    Serial.print("  suhu air    : "); Serial.println(temperature);
    Serial.print("  suhu ruang  : "); Serial.println(ambient);
  } 
}