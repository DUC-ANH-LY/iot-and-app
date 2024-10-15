int pirSensorPin = 21;
int ledPin = 22;

void setup() {
  pinMode(pirSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200); // Change baud rate for ESP32
}

void loop() {
  int pirValue = digitalRead(pirSensorPin);

  if (pirValue == HIGH) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Motion detected!");
  } else {
    Serial.println("Motion monitoring....");
    digitalWrite(ledPin, LOW);
  }

  delay(100);
}