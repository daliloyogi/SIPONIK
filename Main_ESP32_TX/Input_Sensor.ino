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

void dallas() {
  sensors.requestTemperatures();
  SuhuAir = sensors.getTempCByIndex(0);
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