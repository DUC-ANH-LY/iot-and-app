
/*
 *  Lesson-11
 *  Make sure to install PubSubClient library prior to compiling this sketch
 */
 
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker

#define LED_OFF 0
#define LED_RED 1
#define LED_GREEN 2
#define LED_BLUE 3

/*
ESP8266 pin usage:

  D0:  Relay, When 0 is applied, relay is activated
  D1:  I/O, with Factory reset Push button
  D2:  P0, solid state switch
  D3:  LED_RED
  D4:  LED_GREEN
  D5:  LED_BLUE
  D6:  I/O   ; Will connect motion sensor to this
  D7:  I/O   ; will connect DHT11 temp/humidity sensor to this
  D8:  Buzzer ; whwn 1 is applied, P0 is actrivated
  
*/
WiFiClient wifiClient;
DHT dhtd7(D7, DHT11);

char *ssid = "WiFi-66CF";       // maximum allowable ssid length is 27, as one terminating null character is reserved
char *password = "98490820";   // maximum allowable password lenth is 27
const char* mqtt_username = "raman";
const char* mqtt_password = "Test@2021";

const char* mqtt_server = "35.223.145.82";
const char* clientID = "MyDevice";
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the secure listener port for the Broker

long last_motion_check_time;
int motion_check_duration;
boolean relay_status;
boolean P0_status;
uint16_t led_color;
uint16_t prev_motion_sensor;

void setup()
{ 
  Serial.begin(115200);
  Serial.println();
  
  pinMode(D0, OUTPUT);  
  digitalWrite(D0, HIGH);  // Relay is OFF
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, LOW);  // solid state switch P0 is OFF
  pinMode(D3, OUTPUT);     // LED_RED  ; 1 means off, 0 means ON
  digitalWrite(D3, HIGH);
  pinMode(D4, OUTPUT);     // LED_GREEN; 1 mean off, 0 means on 
  digitalWrite(D4, HIGH);
  pinMode(D5, OUTPUT);     // LED_BLUE; 1 means off, 0 means on
  digitalWrite(D5, HIGH);  
  pinMode(D6, INPUT);
  pinMode(D8, OUTPUT);  // buzzer on this

  dhtd7.begin();
  
  put_led_on(LED_BLUE);

  // Connect to the WiFi
  WiFi.begin(ssid, password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // setCallback sets the function to be called when a message is received.
  client.setCallback(ReceivedMessage);

  // Connect to MQTT Broker
  if (Connect()) {
    Serial.println("Connected Successfully to MQTT Broker!");  
  }
  else {
    Serial.println("Connection Failed!");
  }

  put_led_on(LED_GREEN);
  last_motion_check_time = millis();
  motion_check_duration = 1000;  // Normally, check motion sensor every second
  relay_status = false;
  P0_status = false;
  prev_motion_sensor = 0;
}


void loop() {
   long now;
   uint16_t motion_sensor;
   float temperature, humidity;
   char msg[256];
   
   // check WiFi status
   // if WiFi is not connected, powercycle the device
   if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Lost WiFi connection.  Restarting");
      delay(50);
      ESP.restart();  // this is software reset which may not work all the time
   }
   
   // If the connection is lost, try to connect again
   if (!client.connected()) {
     Connect();
   }
   // client.loop() just tells the MQTT client code to do what it needs to do itself (i.e. check for messages, etc.)
   client.loop();
  
   now = millis();
   if ((now - last_motion_check_time) > motion_check_duration) {  // Do every second
      last_motion_check_time = now;
      motion_check_duration = 1000;
      motion_sensor = read_motion_sensor();
      if (motion_sensor == 1) {
         if (prev_motion_sensor == 0) {   // Note:  Motion sensor output is 1 when motion is detected
            mqtt_client_publish("MOTION_ALARM", "ON");
            beep_buzzer();
            motion_check_duration = 15000;  // If alarm is detected, motion should not be checked earlier than 10 seconds
         }
      }
      prev_motion_sensor = motion_sensor;
   }
}


void put_led_on(uint16_t color) {
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);
  
  if (color == LED_RED) {
      digitalWrite(D3, LOW);
  } else if (color == LED_GREEN) {
      digitalWrite(D4, LOW);
  } else if (color == LED_BLUE) {
      digitalWrite(D5, LOW);
  }
  led_color = color;
}

void put_on_relay() {
   digitalWrite(D0, LOW); // RELAY.  We have inverted D0 in hardware
}

void put_off_relay() {
   digitalWrite(D0, HIGH); // RELAY.  We have inverted D0 in hardware
}

void put_on_P0() {
   digitalWrite(D2, HIGH);
}

void put_off_P0() {
   digitalWrite(D2, LOW);
}

void beep_buzzer() {
   digitalWrite(D8, HIGH);  // produces a beep of 200 msesc duration
   delay(200);
   digitalWrite(D8, LOW);
}

uint16_t read_motion_sensor() {
   return (uint16_t)digitalRead(D6);  // Note: Motion sensor output is 1 when motion is detected
}

float read_dht11_temp() {
   return dhtd7.readTemperature();
}

float read_dht11_hum() {
   return dhtd7.readHumidity();;
}

void ReceivedMessage(char* topic, byte* payload, unsigned int payload_length) {
  // Output the first character of the message to serial (debug)
  char msg[256];
  int i;
  uint16_t motion_sensor;
  float temperature, humidity;
  char* motion_str;
  boolean send_sensors;

  if (payload_length > 0) {
     if (strncmp((char*)payload, "RELAY:OFF", payload_length) == 0) {
        put_off_relay();
     } else if (strncmp((char*)payload, "RELAY:ON", payload_length) == 0) {
        put_on_relay();
     } else if (strncmp((char*)payload, "P0:OFF", payload_length) == 0) {
        put_off_P0();
     } else if (strncmp((char*)payload, "P0:ON", payload_length) == 0) {
        put_on_P0();
     } else if (strncmp((char*)payload, "LED:RED", payload_length) == 0) {
        put_led_on(LED_RED);
     } else if (strncmp((char*)payload, "LED:GREEN", payload_length) == 0) {
        put_led_on(LED_GREEN);
     } else if (strncmp((char*)payload, "LED:BLUE", payload_length) == 0) {
        put_led_on(LED_BLUE);
     } else if (strncmp((char*)payload, "LED:OFF", payload_length) == 0) {
        put_led_on(LED_OFF);
     } else if (strncmp((char*)payload, "BUZZER:ON", payload_length) == 0) {
        beep_buzzer();
     }
  }

  motion_sensor = read_motion_sensor();
  if (motion_sensor == 0) {
     motion_str = "OFF";
  } else {
     motion_str = "ON";
  }
  temperature = read_dht11_temp();
  humidity = read_dht11_hum();
  sprintf(msg, "Motion: %s,  Temperature: %3.2f, Humidity: %3.2f", motion_str, temperature, humidity);
  mqtt_client_publish("SENSORS", msg);
}

bool Connect() {
  // Connect to MQTT Server and subscribe to the topic
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
      client.subscribe("DEVICE");
      return true;
    }
    else {
      return false;
  }
}

boolean mqtt_client_publish(char* topic, char* msg) {
   boolean result;

   result = client.publish(topic, msg);
   if (!result) {
      Serial.println("Connected, but could not publish");
   }
   return result;
}
