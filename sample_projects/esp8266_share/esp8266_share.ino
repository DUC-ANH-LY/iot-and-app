#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define SSID "TP-Link_3094"
#define PASSWORD "quenmatkhau"
#define MQTT_SERVER "192.168.0.101"
#define MQTT_PORT 1883

#define FANPIN D5
#define LEDPIN D3
#define DHTPIN D6

WiFiClient espClient;
PubSubClient mqttClient(espClient);
DHT dht(DHTPIN, DHT11);

unsigned long startDhtTime = 0;
int ledState = LOW;
int fanState = LOW;

void setup()
{
  Serial.begin(115200);

  dht.begin();
  pinMode(LEDPIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);
  randomSeed(micros());
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
  Serial.println(WiFi.localIP());
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.connect("esp8266");
  mqttClient.setCallback(sub_data);
}

void sub_data(char *topic, byte *payload, unsigned int length)
{
  if (strcmp(topic, "button") == 0)
  {
    String s = "";
    for (int i = 0; i < length; i++)
      s += (char)payload[i];
    if (s.equalsIgnoreCase("led|on"))
    {
      ledState = HIGH;
    }
    if (s.equalsIgnoreCase("led|off"))
    {
      ledState = LOW;
    }
    if (s.equalsIgnoreCase("fan|on"))
    {
      fanState = HIGH;
    }
    if (s.equalsIgnoreCase("fan|off"))
    {
      fanState = LOW;
    }
    digitalWrite(LEDPIN, ledState);
    digitalWrite(FANPIN, fanState);
    Serial.println(s);
    mqttClient.publish("action", s.c_str());
  }
}

int readAnalog(int pin)
{
  return analogRead(pin);
}
void loop()
{
  unsigned long currentMillis = millis();
  if (mqttClient.connected())
  {
    mqttClient.loop();
    mqttClient.subscribe("button");
    if (currentMillis - startDhtTime >= 2000)
    {
      startDhtTime = currentMillis;
      float temp = dht.readTemperature();
      float humi = dht.readHumidity();
      // int light = random(0, 100);
      // Chân 3v3 cho ra 0 -> 1024 hoặc Chân GND cho ra 1024 -> 0
      // Chân bắt buộc là A0
      int light = readAnalog(A0);

      String data = "DHT11|" + String(temp) + "|" + String(humi) + "|" + String(light);
      Serial.println(data);
      mqttClient.publish("sensor", data.c_str());
    }
  }
}