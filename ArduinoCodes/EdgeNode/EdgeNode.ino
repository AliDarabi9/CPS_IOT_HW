#include <Servo.h>

Servo myservo;
int moistureSensorValue;
int servoPosition = 1500;
int m1 = 13, m2 = 12, en = 3;  // Changed en to pin 3
int pot = A1, st = 0;

void setup() {
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(en, OUTPUT);

  myservo.attach(9);
  Serial.begin(9600);

  Serial.println("Slave Ready");
  delay(2000);
  Serial.println();
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

  // Serial Display (Virtual Terminal)
  Serial.print("Moisture (V): ");
  Serial.println(SensorVolts, 2);

  Serial.print("Servo Angle: ");
  Serial.println(angle);

  Serial.print("Motor Speed: ");
  Serial.println(st);

  Serial.println("-------------------------");

  delay(500);
}
