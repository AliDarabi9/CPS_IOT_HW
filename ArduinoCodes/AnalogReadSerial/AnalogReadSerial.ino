// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode (10,OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.println(sensorValue);

  if (sensorValue<15)
  {
    digitalWrite (10,HIGH);
  }
  else
  {
    digitalWrite (10,LOW);
  }
  delay(500);    // delay in between reads for stability
}
