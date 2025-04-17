#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD I2C address: usually 0x27 or 0x3F
LiquidCrystal_I2C lcd(0x3F, 20, 4);

int lightSensorLeft = 0;
int lightSensorRight = 0;
int thresholdForLightSensors = 5;
int temperatureSensor;

void setup() {
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensors Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  lightSensorLeft = analogRead(A0);
  lightSensorRight = analogRead(A1);
  temperatureSensor = analogRead(A2);

  int diff = lightSensorLeft - lightSensorRight;

  // Simulated servo positions (you can add servo control later)
  int pos;
  if (diff > thresholdForLightSensors) {
    pos = 1167;
  } else if (diff < -thresholdForLightSensors) {
    pos = 1835;
  } else {
    pos = 1500;
  }

  // Temperature conversion
  float mv = (temperatureSensor / 1024.0) * 5000.0;  // in millivolts
  float cel = mv / 10.0; // LM35: 10mV per degree Celsius

  // Display on LCD
  lcd.setCursor(0, 0);
  lcd.print("Left Light : ");
  lcd.print(lightSensorLeft);

  lcd.setCursor(0, 1);
  lcd.print("Right Light: ");
  lcd.print(lightSensorRight);

  lcd.setCursor(0, 2);
  lcd.print("Temp (C): ");
  lcd.print(cel, 1); // one decimal place

  lcd.setCursor(0, 3);
  lcd.print("Servo Pos: ");
  lcd.print(pos);

  delay(500);
}
