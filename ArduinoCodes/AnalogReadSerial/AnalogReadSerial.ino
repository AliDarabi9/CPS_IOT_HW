#include <Servo.h>

Servo myservo1;
Servo myservo2;

int posLeft = 1500;
int posRight = 1500;
int sensorValue1 = 0;
int sensorValue2 = 0;
int threshold = 10;  // adjust for sensitivity

void setup() {
  Serial.begin(9600);
  myservo1.attach(9);  // First servo on pin 9
  myservo2.attach(10); // Second servo on pin 10

  myservo1.write(posRight);
  myservo2.write(posLeft); // Initialize second servo to same position
}

void loop() {
  sensorValue1 = analogRead(A0);
  sensorValue2 = analogRead(A1);

  Serial.print("Sensor 1: ");
  Serial.print(sensorValue1);
  Serial.print(", Sensor 2: ");
  Serial.print(sensorValue2);

  int diff = sensorValue1 - sensorValue2;

  if (diff > threshold) {
    posLeft = 1165;  // LDR1 is significantly brighter
    posRight = 1165;  // LDR1 is significantly brighter
  } else if (diff < -threshold) {
    posLeft = 1835; // LDR2 is significantly brighter
    posRight = 1835; // LDR2 is significantly brighter
  } else {
    posLeft = 1500;  // Light is about equal
    posRight = 1500;  // Light is about equal
  }

  Serial.print(", Servo Left Position: ");
  Serial.println(posLeft);
  Serial.print(", Servo Right Position: ");
  Serial.println(posRight);

  myservo1.writeMicroseconds(posLeft);
  myservo2.writeMicroseconds(posRight);  // Move second servo in sync (optional)
  
  delay(50);
}
