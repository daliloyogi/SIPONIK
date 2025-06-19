void IntroDisplay() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, Light);
  lcd.createChar(1, Degree);

  for (int i = 0; i <= 16; i++) {
    lcd.setCursor(i, 1);
    lcd.write(byte(0));
    delay(90);
    lcd.clear();
  }

  lcd.setCursor(4, 0);
  lcd.print("SIPONIK");
  delay(2000);
  lcd.clear();

  for (int i = 16; i >= 0; i--) {
    lcd.setCursor(i, 1);
    lcd.write(byte(0));
    delay(90);
    lcd.clear();
  }
}

void MainDisplay() {
  lcd.setCursor(0, 0); lcd.print(SuhuAir,1);
  lcd.setCursor(4, 0); lcd.print(byte(1));
  lcd.setCursor(5, 0); lcd.print("C");

  lcd.setCursor(0, 1); lcd.print("INDEX PH");
  lcd.setCursor(3, 1); lcd.print(pH,1);

  lcd.setCursor(0, 2); lcd.print(KetinggianAir);
  lcd.setCursor(4, 2); lcd.print("%");

  lcd.setCursor(0, 3); lcd.print(Nutrisi);
  lcd.setCursor(5, 3); lcd.print("PPM");
}

void SelenoidOnDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Volume Air Rendah!");
  lcd.setCursor(0, 1); lcd.print("Selenoid On!");
}