#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


#define PIN_RED    18 // GPIO23
#define PIN_GREEN  19 // GPIO22
#define PIN_BLUE   21 // GPIO21



// Add wifi network
const char* ssid = "Duc Anh 2003";
const char* password = "Ducanhdeptrai172@";

// Add your MQTT Broker IP address and credentials
const char* mqtt_server = "192.168.1.11";
const char* mqtt_username = "ducanh";
const char* mqtt_password = "ducanh172";

// Create service client object
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


void setup() {
  Serial.begin(115200);
  
  setup_wifi();
  setup_mqtt();


  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
}


void setup_mqtt() {
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);
}



void setup_wifi() {
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
  // String messageTemp;
  
  // for (int i = 0; i < length; i++) {
  //   Serial.print((char)message[i]);
  //   messageTemp += (char)message[i];
  // }
  // Serial.println();
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

  Serial.println((int)doc["color"][0]);

  setColor(doc["color"][0], doc["color"][1], doc["color"][2]);


  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  // if (String(topic) == "esp32/output") {
  //   Serial.print("Changing output to ");
  //   if(messageTemp == "on"){
  //     Serial.println("on");
  //     digitalWrite(ledPin, HIGH);
  //   }
  //   else if(messageTemp == "off"){
  //     Serial.println("off");
  //     digitalWrite(ledPin, LOW);
  //   }
  // }
}



void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // boolean connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage], [cleanSession])
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




void loop() {

  if (!mqttClient.connected()) {
    reconnect();
  }

  // maintain connection
  mqttClient.loop();

  // color code #00C9CC (R = 0,   G = 201, B = 204)
  // setColor(0, 201, 204);

  // delay(1000); // keep the color 1 second

  // // color code #F7788A (R = 247, G = 120, B = 138)
  // setColor(247, 120, 138);

  // delay(1000); // keep the color 1 second

  // // color code #34A853 (R = 52,  G = 168, B = 83)
  // setColor(52, 168, 83);

  // delay(1000); // keep the color 1 second
}

void setColor(int R, int G, int B) {
  analogWrite(PIN_RED,   R);
  analogWrite(PIN_GREEN, G);
  analogWrite(PIN_BLUE,  B);
}
