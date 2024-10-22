#include "DHT.h"

#include <Wire.h>  

#include <ESP8266WiFi.h> 

#include <ESP8266HTTPClient.h>

//Initialisations for DHT11 Sensor

#define DHTPIN 0

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE); 

float Temperature;

float Humidity;

//End Of Initialisations for DHT11 Sensor

//Initialisations for SSD1306 Sensor

#define SCREEN_WIDTH 128 // OLED display width, in pixels

#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for the SSD1306 display connected via I2C -> SDA + SCL Pins

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//"&Wire" Declares the I2C Communication Protocol 

//"-1" tells that the OLED display has no RESET Pin which is the case with our OLED Display.

//End Of Initialisations for SSD1306 Sensor

// Declare global variables which will be uploaded to server

String sendval, sendval2, postData;

void setup() {

  Serial.begin(115200); 

  Serial.println("Communication Started \n\n");  

  delay(1000);


}

void loop(){ 


  HTTPClient http;    // http object of clas HTTPClient

  // Convert to float

  sendval = 2.3;  

  sendval2 = 3.4;   

  postData = "sendval=" + 2.3 + "&sendval2=" + 3.4;

  // We can post values to PHP files as  example.com/dbwrite.php?name1=val1&name2=val2

  // Hence created variable postData and stored our variables in it in desired format

  // Update Host URL here:-  

  http.begin("http://localhost:4444/dbwrite.php");                             // Connect to host where MySQL database is hosted

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");        //Specify content-type header

  int httpCode = http.POST(postData);   // Send POST request to php file and store server response code in variable named httpCode

  Serial.println("Values are, sendval = " + sendval + " and sendval2 = "+sendval2 );

  // if connection eatablished then do this

  if (httpCode == 200) { Serial.println("Values uploaded successfully."); Serial.println(httpCode); 

    String webpage = http.getString();    // Get html webpage output and store it in a string

    Serial.println(webpage + "\n");

  } else { 

    // if failed to connect then return and restart

    Serial.println(httpCode); 

    Serial.println("Failed to upload values. \n"); 

    http.end(); 

    return; 

  }

}