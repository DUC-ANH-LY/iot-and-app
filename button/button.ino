const int ledPin1 = 19; // Pin number where the first LED is connected
const int buttonPin1 = 23; // Pin number where the first button is connected


void setup() {
  pinMode(ledPin1, OUTPUT); // Set the first LED pin as an OUTPUT
  pinMode(buttonPin1, INPUT_PULLUP); // Set the first button pin as an INPUT with internal pull-up resistor

  digitalWrite(ledPin1, LOW); // Turn the first LED OFF initially
}

void loop() {
  // Check if the first button is pressed (LOW state when pressed due to the internal pull-up resistor)
  if (digitalRead(buttonPin1) == LOW) {
    digitalWrite(ledPin1, HIGH); // Turn the first LED ON when the first button is pressed
  } else {
    digitalWrite(ledPin1, LOW); // Turn the first LED OFF when the first button is released
  }
}