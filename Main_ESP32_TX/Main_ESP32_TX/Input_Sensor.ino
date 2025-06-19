void ultrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  KetinggianAir = (depthCm - distanceCm) / depthCm * 100;
}

float dallas() {
  byte data[9];
  byte addr[8];

  if (!ds.search(addr)) {
    ds.reset_search();
    return -1000;  // Error: sensor not found
  }

  // Verify CRC check
  if (OneWire::crc8(addr, 7) != addr[7]) {
    return -1000;  // Error: invalid address
  }

  // Check if device is a DS18B20
  if (addr[0] != 0x10 && addr[0] != 0x28) {
    return -1000;  // Error: not a DS18B20
  }

  // Start temperature conversion
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);  // Start conversion

  // Read Scratchpad
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);  // Read scratchpad

  for (int i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  int16_t rawTemperature = (data[1] << 8) | data[0];  // Combine bytes
  return rawTemperature / 16.0;                       // Convert to °C
}

void PH() {
  int adcRawPH = analogRead(PH_PIN);
  voltage = adcRawPH / 4095.0 * 3300.0;
  pH = ph.readPH(voltage, SuhuAir);
}

void TDS() {
  int adcRawTDS = analogRead(TDS_PIN);
  tdsVoltage = adcRawTDS / 4095.0 * 3.3;
  Nutrisi = (133.42 * tdsVoltage * tdsVoltage * tdsVoltage - 255.86 * tdsVoltage * tdsVoltage + 857.39 * tdsVoltage) * 0.5;
}