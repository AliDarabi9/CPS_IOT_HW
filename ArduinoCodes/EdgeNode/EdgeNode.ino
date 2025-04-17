#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo myservo;
int moistureSensorValue;
int servoPosition = 1500;

// 20x4 LCD, address 0x27 (try 0x3F if it doesn’t work)
LiquidCrystal_I2C lcd(0x3F, 20, 4);

void setup() {
  moistureSensorValue = analogRead(A0);     
  myservo.attach(9); 
  Serial.begin(9600); 

  lcd.init();        // Correct for I2C LCD
  delay(100);        // Let it initialize properly
  lcd.backlight();   // Turn on the backlight
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}


void loop() {
  float SensorVolts = analogRead(A0) * 0.0048828125;
  int angle = map(servoPosition, 1000, 2000, 0, 180);

  Serial.print("Servo Position: ");
  Serial.println(angle);

  lcd.setCursor(0, 0);
  lcd.print("Moisture:");
  lcd.print(SensorVolts, 2);

  lcd.setCursor(0, 1);
  lcd.print("Servo:");
  lcd.print(angle);
  lcd.print((char)223); // degree symbol

  myservo.write(servoPosition);
  delay(500);
}
