
#include <Wire.h>

int lightSensorLeft = 0;
int lightSensorRight = 0;
int thresholdForLightSensors = 5;
int temperatureSensor;

void setup() {
  Wire.begin();  // Join I2C bus as master
  Serial.begin(9600);
  Serial.println("Master Ready");
  delay(2000);
  Serial.println();
}

void loop() {
  Wire.requestFrom(8, 1);
  int slaveValue = -1;
  while (Wire.available()) {
    slaveValue = Wire.read();
    Serial.print("Moisture: ");
    Serial.println(slaveValue);
  }

  lightSensorLeft = analogRead(A0);
  lightSensorRight = analogRead(A1);
  temperatureSensor = analogRead(A2);

  int diff = lightSensorLeft - lightSensorRight;

  // Simulated servo positions
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

  // Display on Serial Monitor (Virtual Terminal)
  Serial.print("Left Light : ");
  Serial.println(lightSensorLeft);

  Serial.print("Right Light: ");
  Serial.println(lightSensorRight);

  Serial.print("Temp (C): ");
  Serial.println(cel, 1); // one decimal place

  Serial.print("Servo Pos: ");
  Serial.println(pos);

  Serial.println("------------------------");

  delay(500);
}
