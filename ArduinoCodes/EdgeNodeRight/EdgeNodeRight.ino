#include <Servo.h>
#include <Wire.h>

Servo myservo;
int moistureSensorValue;
float SensorVolts; 
int servoPosition = 1500;
int m1 = 13, m2 = 12, en = 3; 
int motorSpeed = 0;

int data = 128;
int commandCNT = 0;


void setup() {
  Wire.begin(9);            // Join I2C bus with address 9
  Wire.onRequest(requestEvent);  // Register request event
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);       // Start Serial for debugging

  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(en, OUTPUT);

  myservo.attach(9);

  Serial.println("<Edge Node Right>");
  delay(2000);
  Serial.println();
}

void loop() {
  moistureSensorValue = analogRead(A0);
  SensorVolts = moistureSensorValue * 0.0048828125;

  data = convertVolts(SensorVolts);

  myservo.write(servoPosition);

  analogWrite(en, motorSpeed);
  digitalWrite(m1, HIGH);
  digitalWrite(m2, LOW);

  Serial.print("Moisture (V): ");
  Serial.println(SensorVolts, 2);

  Serial.print("Servo Angle: ");
  int angle = map(servoPosition, 1000, 2000, 0, 180);
  Serial.println(angle);

  Serial.print("Motor Speed: ");
  Serial.println(motorSpeed);

  Serial.println("-------------------------");

  delay(500);
}

int convertVolts(float input) {
  int multipliedInt = input * 100;
    // Map from original range (0-500) to (128-255)
  int mappedValue = map(multipliedInt, 0, 500, 128, 255);
    // Apply your special mapping rules
  int finalValue;
  if (mappedValue <= 129) {
    finalValue = 128;
  } else if (mappedValue >= 254) {
    finalValue = 254;
  } else {
    if (mappedValue % 2 == 0) {
      finalValue = mappedValue;
    } 
    else {
      finalValue = mappedValue + 1;
    }
  }
  return finalValue;
}

void requestEvent() {
  Wire.write(data); 
}

void receiveEvent() {
  while (Wire.available()) {
    int c = Wire.read();
    if (commandCNT % 2 == 0) {
      servoPosition = 1167 + ((c - 1) * 333);
      Serial.print("Servo Command: ");
      Serial.println(c);
    } else {
      motorSpeed = (c - 1) * 50;
      Serial.print("Motor Command: ");
      Serial.println(c);
    }
    commandCNT += 1;
    Serial.println("-------------------------");
 }
}

