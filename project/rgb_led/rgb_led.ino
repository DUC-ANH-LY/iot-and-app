#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


#define PIN_RED    18 // GPIO23
#define PIN_GREEN  19 // GPIO22
#define PIN_BLUE   21 // GPIO21
#define PIN_LIGHT_SENSOR   15
#define PIN_MOTION_SENSOR  22
#define PIN_SWITCH_ENABLE  23
#define PIN_SWITCH_DISABLE  5


// Add wifi network
const char* ssid = "Duc Anh 2003";
const char* password = "Ducanhdeptrai172@";

// Add your MQTT Broker IP address and credentials
const char* mqtt_server = "mqtt-dashboard.com";
const char* mqtt_username = "ducanh";
const char* mqtt_password = "ducanh172";

// Create service client object
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


void setup() {
  Serial.begin(115200);
  setUpWifi();
  setUpMqtt();
  setUpInitValue();
  setUpPinMode();
}

void setUpInitValue() {
  setColor(0,0,0);
}

void setUpPinMode() {
  pinMode(PIN_LIGHT_SENSOR,INPUT);
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
  pinMode(PIN_SWITCH_ENABLE,  INPUT_PULLUP);
  pinMode(PIN_SWITCH_DISABLE,  INPUT_PULLUP);
  pinMode(PIN_MOTION_SENSOR, INPUT);
}


void setUpMqtt() {
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);
}



void setUpWifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
   
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }

  JsonDocument doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, message);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Serial.println((int)doc["color"][0]);
  if (String(topic) == "setRgb") {
    Serial.print("Set Rgb Message");
    setColor(doc["color"][0], doc["color"][1], doc["color"][2]);
  }
}



void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("esp32Client123", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Subscribe
      mqttClient.subscribe("setRgb");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


int R=0,G=0,B=0;
int preR=0,preG=0,preB=0;
int motionSensorValue=0,lightSensorValue=0;
unsigned long startTime = 0;
char message[1000];
int startTimeTurnOnLed  = 0;
int duration = 0;
JsonDocument doc;


void loop() {
  unsigned long currentMillis = millis();



  if (!mqttClient.connected()) {
    reconnect();
  }

  // maintain connection
  mqttClient.loop();

  int switchValue1 = digitalRead(PIN_SWITCH_ENABLE);
  int switchValue2 = digitalRead(PIN_SWITCH_DISABLE);
  // click button
  if (switchValue1 == 0) {
    if (isLedoff()) {
      setColor(255,250,250);
      Serial.println("Led on");
    }
  }
  if (switchValue2 == 0) {
    setColor(0,0,0);
    Serial.println("Led off");
  }
  motionSensorValue = digitalRead(PIN_MOTION_SENSOR);
  lightSensorValue = digitalRead(PIN_LIGHT_SENSOR);

  if (isLedoff()) {
    // light sensor
    if(lightSensorValue == HIGH 
    ){
        toggleLed("Light sensor detect");
    }
    if (motionSensorValue == HIGH) {
        toggleLed("Motion sensor detect");
    }
  }


  if (duration > 0) {
    doc["motion_sensor_value"] = motionSensorValue;
    doc["light_sensor_value"] = lightSensorValue;
    doc["getRgb"]["R"] = preR;
    doc["getRgb"]["G"] = preG;
    doc["getRgb"]["B"] = preB;
    doc["getRgb"]["duration"] = duration;
    duration = 0;
    char newMessage[1000];
    serializeJson(doc, newMessage);

    Serial.println("Publish mqtt message updateData");
    mqttClient.publish("updateData", newMessage);
  }

}


void toggleLed(String sensor) {
  Serial.println(sensor);
  Serial.println("Turn Led on");
  // turn on 
  setColor(255,250,250);
  delay(1500);
  // turn off
  setColor(0,0,0);
}

void setColor(int RR, int GG, int BB) {
  // last rgb status led on
  preR = R;
  preG = G;
  preB = B;
  analogWrite(PIN_RED,   RR);
  analogWrite(PIN_GREEN, GG);
  analogWrite(PIN_BLUE,  BB);
  // set rgb value
  R = RR;
  G = GG;
  B = BB;

  // publish sync data
  doc["motion_sensor_value"] = motionSensorValue;
  doc["light_sensor_value"] = lightSensorValue;
  doc["getRgb"]["R"] = R;
  doc["getRgb"]["G"] = G;
  doc["getRgb"]["B"] = B;
  doc["getRgb"]["duration"] = NULL;
  char newMessage1[1000];
  serializeJson(doc, newMessage1);
  Serial.println("Publish mqtt message syncData");
  Serial.println(newMessage1);
  mqttClient.publish("syncData", newMessage1);

  // caculate light time
  if (isLedoff()) {
    if (startTimeTurnOnLed != 0) {
       duration = millis() - startTimeTurnOnLed;
    }
    startTimeTurnOnLed  = 0;
  }
  else {
    startTimeTurnOnLed  = millis();
    duration = 0 ;
  }
  Serial.println(duration);
}


bool isLedoff() {
  // Serial.print("R=");
  // Serial.println(R);
  return (R == 0) && (G == 0) && (B == 0);
  
}
