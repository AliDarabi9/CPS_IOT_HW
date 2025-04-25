#include <Servo.h>
#include <Wire.h>

const int I2C_ADDRESS = 9;

const float VOLTAGE_CONVERSION = 0.0048828125;
const int SERVO_MIN_PULSE = 1000;
const int SERVO_MAX_PULSE = 2000;

Servo myservo;
int moistureSensorValue = 0;
float sensorVoltage = 0.0;
int servoPosition = 1500;
int motorSpeed = 0;
int dataToSend = 128;
int commandCNT = 0;

void setup() {
  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  Serial.begin(9600);
  Serial.println("<Edge Node Right>");
  delay(2000);
  Serial.println();

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(3, OUTPUT);

  myservo.attach(9);
}

void loop() {
  readMoistureSensor();
  updateActuators();
  logStatus();
  delay(500);
}

void readMoistureSensor() {
  moistureSensorValue = analogRead(A0);
  sensorVoltage = moistureSensorValue * VOLTAGE_CONVERSION;
  dataToSend = converter(sensorVoltage);
}

void updateActuators() {
  myservo.write(servoPosition);
  analogWrite(3, motorSpeed);
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
}

void logStatus() {
  Serial.print("Moisture (V): ");
  Serial.println(sensorVoltage, 2);

  Serial.print("Servo Angle: ");
  int angle = map(servoPosition, SERVO_MIN_PULSE, SERVO_MAX_PULSE, 0, 180);
  Serial.println(angle);

  Serial.print("Motor Speed: ");
  Serial.println(motorSpeed);
  Serial.println("-------------------------");
}

int converter(float input) {
  int value = input * 100;
  int mappedValue = map(value, 0, 500, 128, 255);

  if (mappedValue <= 129) return 128;
  if (mappedValue >= 254) return 254;
  return (mappedValue % 2 == 0) ? mappedValue : mappedValue + 1;
}

void requestEvent() {
  Wire.write(dataToSend);
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
    commandCNT++;
    Serial.println("-------------------------");
  }
}
