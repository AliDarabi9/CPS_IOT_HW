
#include <Wire.h>

int lightSensorLeft = 0;
int lightSensorRight = 0;
int thresholdForLightSensors = 3;
int diff = 0;
int pos = 90;
int temperatureSensor;
int turnCNT = 0;
int moisturePercentageLeftVolt = -1;
int moisturePercentageLeft = -1;
int moisturePercentageRightVolt = -1;
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
    Wire.requestFrom(9, 1);
    while (Wire.available()) {
      moisturePercentageRightVolt = Wire.read();
    }
    Wire.beginTransmission(8);
    if (diff > thresholdForLightSensors) {
      // pos = 1167;
      pos = 30;
      Wire.write(1);
    } else if (diff < -thresholdForLightSensors) {
      // pos = 1835;
      pos = 150;
      Wire.write(3);
    } else {
      // pos = 1500;
      pos = 90;
      Wire.write(2);
    }
    if (moisturePercentageLeft < 50) {
      Wire.write(4);
    } else if (80 < moisturePercentageLeft) {
      Wire.write(1);
    } else {
      if (temperatureCelsius > 25) {
        Wire.write(3);
      } else {
        Wire.write(2);
      }
    }
    Wire.endTransmission();
  }
  else {
    Wire.requestFrom(8, 1);
    while (Wire.available()) {
      moisturePercentageLeftVolt = Wire.read();
    }
    if (turnCNT > 0) { 
      Wire.beginTransmission(9);
      if (diff > thresholdForLightSensors) {
        // pos = 1167;
        pos = 30;
        Wire.write(1);
      } else if (diff < -thresholdForLightSensors) {
        // pos = 1835;
        pos = 150;
        Wire.write(3);
      } else {
        // pos = 1500;
        pos = 90;
        Wire.write(2);
      }
      if (moisturePercentageRight < 50) {
        Wire.write(4);
      } else if (80 < moisturePercentageRight) {
        Wire.write(1);
      } else {
        if (temperatureCelsius > 25) {
          Wire.write(3);
        } else {
          Wire.write(2);
        }
      }
      Wire.endTransmission();
    }
  }

  moisturePercentageLeft = converter(moisturePercentageLeftVolt);
  moisturePercentageRight = converter(moisturePercentageRightVolt);
  Serial.print("Moisture Percentage Left: ");
  Serial.println(moisturePercentageLeft);
  Serial.print("Moisture Percentage Right: ");
  Serial.println(moisturePercentageRight);

  lightSensorLeft = analogRead(A0);
  lightSensorRight = analogRead(A1);
  temperatureSensor = analogRead(A2);

  diff = lightSensorLeft - lightSensorRight;


  // Temperature conversion
  float mv = (temperatureSensor / 1024.0) * 5000.0;  // in millivolts
  temperatureCelsius = mv / 10.0; // LM35: 10mV per degree Celsius

  // Display on Serial Monitor (Virtual Terminal)
  Serial.print("Left Light : ");
  Serial.println(lightSensorLeft);

  Serial.print("Right Light: ");
  Serial.println(lightSensorRight);

  Serial.print("Temp (C): ");
  Serial.println(temperatureCelsius, 1); // one decimal place

  Serial.print("Servo Pos: ");
  Serial.println(pos);

  Serial.println("------------------------");

  turnCNT += 1;

  delay(500);
}

int converter(int input) {
  int outputPercent = (input - 192) * 100;
  outputPercent = outputPercent / (255 - 192);
  return outputPercent;
}
