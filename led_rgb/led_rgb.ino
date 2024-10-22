#define BLYNK_TEMPLATE_ID "TMPLjOi0Rz-W"
#define BLYNK_TEMPLATE_NAME "esp led rgb"
#define BLYNK_DEVICE_NAME "esp32"
#define BLYNK_AUTH_TOKEN "Kywmf9kiMHNGCWywWM9OMdl6VxfMaRsR"


#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Duc Anh 2003";
char pass[] = "Ducanhdeptrai172@";


// The state of the LED 
int state = 0;
// The pin number of the LED 
#define pinLed 19
// Create an object of type BlynkerTimer
BlynkTimer timer;

// Predifined function to read instructions from Blynk 
BLYNK_WRITE(V1)
{
  state = param.asInt();
  if(state == 1)
  {
    digitalWrite(pinLed, HIGH);
    Serial.write("LED Stopped...");
    Blynk.virtualWrite(V1,HIGH);
  }else{
    digitalWrite(pinLed, LOW);
    Serial.write("LED Started...");
    Blynk.virtualWrite(V1,LOW);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  pinMode(pinLed, OUTPUT);
  Blynk.begin(auth,ssid,pass);
}

void loop() {

  Blynk.run();
  timer.run();
}
