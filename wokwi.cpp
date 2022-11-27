/**
   ESP32 + DHT22 Example for Wokwi + 
   
   https://wokwi.com/arduino/projects/322410731508073042
   and 
   https://www.youtube.com/watch?v=C1yz7dTzfVQ 

  all together by Camis

  incluir libraries
  Thingspeak
  DHT sensor library for ESPx
  Adafruit Sensor Lab
*/

#include <WiFi.h>
#include "DHTesp.h"
#include "ThingSpeak.h"
#include <Adafruit_Sensor.h>

const int DHT_PIN = 15;
const char* ssid = "Wokwi-GUEST"; // <-- inserir wifi
const char* password = ""; // <-- inserir senha wifi
 
 WiFiClient client;

// thingspeak API 
 unsigned long myChannelNumber = 2;
 const char* myApiKey = "66G575XU3QRFEXGW"; 
 const char* server = "api.thingspeak.com"; 
 
 unsigned long lastTime = 0;
 unsigned long timeDelay = 10000;

 int tempC;
 int humidity;

DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  dhtSensor.getPin();
  delay(10);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println(".");
  }
  Serial.println("wifi connected !");
  Serial.println(WiFi.localIP());
  
  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);

}

void loop() {
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature, 1) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 2) + "%");
  Serial.println("---");

tempC = dhtSensor.getTemperature();
 Serial.println("Temperature C ");
  Serial.println(tempC);
 Serial.println(" °C ");

humidity = dhtSensor.getHumidity();
Serial.println(" Humidity " );
   Serial.println(humidity);
 Serial.println(" % ");

ThingSpeak.setField(1, tempC);
ThingSpeak.setField(2, humidity);

int x = ThingSpeak.writeFields(myChannelNumber, myApiKey);

if(x == 200){
  Serial.println("foi pro thingSpeak ! ");
}
else{
  Serial.println("perai ! " + String(x));
}

  delay(1000);
}




