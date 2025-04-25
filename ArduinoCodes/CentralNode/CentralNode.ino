#include <Wire.h>

int lightSensorLeft = 0;
int lightSensorRight = 0;
int thresholdForLightSensors = 3;
int lightSensorsDiff = 0;
int flowerPotPosition = 90;
int turnCNT = 0;
int moistureLeft = -1;
int moisturePercentageLeft = -1;
int moistureRight = -1;
int moisturePercentageRight = -1;
float temperatureCelsius = 0;

void setup() {
  Wire.begin();  // Join I2C bus as master
  Serial.begin(9600);
  Serial.println("<Central Node>");
  delay(2000);
  Serial.println();
}

void loop() {
  if (turnCNT % 2 == 1) {
    moistureRight = readMoisture(9);
    sendCommands(8, moisturePercentageLeft);
  } else {
    moistureLeft = readMoisture(8);
    if (turnCNT > 0) { 
      sendCommands(9, moisturePercentageRight);
    }
  }
  moisturePercentageLeft = convertToPercentage(moistureLeft);
  moisturePercentageRight = convertToPercentage(moistureRight);
  readLightSensors();
  readTemperature();
  logStatus();

  turnCNT += 1;
  delay(500);
}

int convertToPercentage(int input) {
  int percentage = (input - 192) * 100;
  percentage = percentage / (255 - 192);
  return percentage;
}

void sendCommands(int addr, int moisturePercentage) {
  Wire.beginTransmission(addr);
  if (lightSensorsDiff > thresholdForLightSensors) {
    flowerPotPosition = 30;
    Wire.write(1);
  } else if (lightSensorsDiff < -thresholdForLightSensors) {
    flowerPotPosition = 150;
    Wire.write(3);
  } else {
    flowerPotPosition = 90;
    Wire.write(2);
  }
  if (moisturePercentage < 50) {
    Wire.write(4); // Set Speed to 150
  } else if (80 < moisturePercentage) {
    Wire.write(1); // Set Speed to 0
  } else {
    if (temperatureCelsius > 25) {
      Wire.write(3); // Set Speed to 100
    } else {
      Wire.write(2); // Set Speed to 50
    }
  }
  Wire.endTransmission();
}

int readMoisture(int addr) {
  Wire.requestFrom(addr, 1);
  while (Wire.available()) {
    return Wire.read();
  }
  return -1;
}

void readLightSensors() {
  lightSensorLeft = analogRead(A0);
  lightSensorRight = analogRead(A1);
  lightSensorsDiff = lightSensorLeft - lightSensorRight;
}

void readTemperature() {
  int sensorValue = analogRead(A2);
  float millivolts = (sensorValue / 1024.0) * 5000.0;
  temperatureCelsius = millivolts / 10.0;
}

void logStatus() {
  Serial.print("Moisture Percentage Left: ");
  Serial.println(moisturePercentageLeft);
  Serial.print("Moisture Percentage Right: ");
  Serial.println(moisturePercentageRight);
  Serial.print("Left Light : ");
  Serial.println(lightSensorLeft);
  Serial.print("Right Light: ");
  Serial.println(lightSensorRight);
  Serial.print("Temp (C): ");
  Serial.println(temperatureCelsius, 1);
  Serial.print("Servo Pos: ");
  Serial.println(flowerPotPosition);
  Serial.println("------------------------");
}
