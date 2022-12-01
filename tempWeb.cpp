#include <WiFi.h>
#include "DHTesp.h"


const char* ssid = "...."; // <-- inserir wifi
const char* password = "..."; // <-- inserir senha wifi


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//temperatura infos
const int DHT_PIN = 15;
DHTesp dhtSensor;


 // thingspeak
// String host     = "api.thingspeak.com"; // Open Weather Map API
// const int httpPort   = 80;
// String uri     = "/update?api_key=5SXNU1Q3CXE33SAD&field1=";


void setup() {
 
Serial.begin(115200); 
    Serial.println("Inicio....."); // mostra texto no serial
    delay(10);

 dhtSensor.setup(DHT_PIN, DHTesp::DHT11);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


void loop() {
  
  //pegar temperatura pro serial
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  String Temperature =  String(data.temperature);
int tempNum ;
tempNum = (int) Temperature;

  String Humidity = String(data.humidity);
  int humidNum ;
  humidNum = (int) Humidity;

  Serial.println("Temp: ") ; Serial.println( tempNum) ; Serial.println( "°C");
  Serial.println("Humidity: " ) ; Serial.println( humidNum) ; Serial.println( "%");
  Serial.println("---");
  delay(1000);


WiFiClient client = server.available();   // Listen for incoming clients


  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

        while (client.connected() ) {  // loop while the client's connected
     

            if (client.available()) {             // if there's bytes to read from the client,
              char c = client.read();             // read a byte, then
              Serial.write(c);                    // print it out the serial monitor
              header += c;

                  if (c == '\n') {                    // if the byte is a newline character
                    if (currentLine.length() == 0) {
                                  client.println("HTTP/1.1 200 OK");
                                  client.println("Content-type:text/html");
                                  client.println("Connection: close");
                                  client.println();


                                  // Display the HTML web page
                                 
                                client.println("<html><head><META HTTP-EQUIV=""refresh""CONTENT=""5"">\n"); 
                                client.println("<title>Temp Web</title></head>"); 
                                client.println("<body>\n");
                                client.println("<h1>Temp Web </h1>"); 
                                client.println("Temperatura:");
                                client.println(humidNum);
                                client.println("<br/>");
                            
                                client.println("Humidade:");
                                client.println(Humidity) ; 
                                client.println("<br/>");
                               
                                client.println("<br/>");
                                client.println("<p> with ♥︎ by Camis - 2022 </p>");



                                  // The HTTP response ends with another blank line
                                  client.println();
                                  // Break out of the while loop
                                  break;
                    } 
                    else { // if you got a newline, then clear currentLine
                      currentLine = "";
                    }
                  } 
                  else if (c != '\r') {  // if you got anything else but a carriage return character,
                      currentLine += c;      // add it to the end of the currentLine
                  }


            }


        }

    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
    }
}
