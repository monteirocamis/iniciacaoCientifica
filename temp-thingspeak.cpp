 //projeto 36 no thinkercad + wearable femRGB c/ nodemcu

#include <ESP8266WiFi.h>
 
const char* ssid = "..."; // <-- inserir wifi
const char* password = "..."; // <-- inserir senha wifi
 
WiFiServer server(80);
 

#define sensorPin 0
float Celsius, Fahrenheit, Kelvin;
int sensorValue;

void setup() {
    Serial.begin(9600); // inicia serial em 9600
    Serial.println("Inicio....."); // mostra texto no serial
    delay(10);


        
    // Connect to WiFi network
    Serial.println();
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
    
    // Start the server
    server.begin();
    Serial.println("Server started");
    
    // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
 
}
void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

    GetTemp();  // le e converte a temperatura e imprime os valores
    Serial.print("Celsius: "); 
    Serial.println(Celsius);
    Serial.print("Fahrenheit: ");
    Serial.println(Fahrenheit); 
    Serial.println();
    delay(2000);
 }
    void GetTemp() { // criacao da funcao getTemp
    sensorValue = analogRead(sensorPin); // le o sensor
    Kelvin = (((float(sensorValue) / 1023) * 5) * 100);     // converte para kelvin
    Celsius = Kelvin - 273.15; // converte para Celsius
    Fahrenheit = (Celsius * 1.8) +32; // converte para Fahrenheit


}