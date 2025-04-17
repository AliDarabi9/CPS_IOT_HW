#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo myservo;
int moistureSensorValue;
int servoPosition = 1500;
int m1 = 13, m2 = 12, en = 3;  // Changed en to pin 3
int pot = A1, st = 0;

LiquidCrystal_I2C lcd(0x3F, 20, 4);

void setup() {
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(en, OUTPUT);
  
  myservo.attach(9);
  Serial.begin(9600); 

  lcd.init();
  delay(100);
  lcd.backlight();
  lcd.print("System Ready");
  delay(500);
  lcd.clear();
}

void loop() {
  moistureSensorValue = analogRead(A0);
  float SensorVolts = moistureSensorValue * 0.0048828125;

  int angle = map(servoPosition, 1000, 2000, 0, 180);
  myservo.write(angle);

  st = analogRead(pot) / 2;  
  analogWrite(en, st);
  digitalWrite(m1, HIGH);
  digitalWrite(m2, LOW);

  // LCD Display
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(SensorVolts, 2);

  lcd.setCursor(0, 1);
  lcd.print("Servo: ");
  lcd.print(angle);
  lcd.print((char)223);

  lcd.setCursor(0, 2);
  lcd.print("Motor Speed: ");
  lcd.print(st);

  delay(100);
}