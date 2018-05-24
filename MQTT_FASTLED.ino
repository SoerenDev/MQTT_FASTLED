#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "FastLED.h"
 
const char* ssid = "foobar";
const char* password =  "";
const char* mqttServer = "10.42.0.244";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

WiFiClient espClient;
PubSubClient client(espClient);



// How many leds in your strip?
#define NUM_LEDS 1

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 5
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];


 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.subscribe("remote");

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  char receivedChar = (char)payload[0];
 
  if(receivedChar == '1') {
    Serial.println("An");
    leds[0] = CRGB ( 0, 0, 255);
  FastLED.show();

  }
  if(receivedChar == '0') {
    Serial.println("Aus");
    leds[0] = CRGB ( 0, 0, 0);
  FastLED.show();

  }

 
}
 
void loop() {
  client.loop();
}