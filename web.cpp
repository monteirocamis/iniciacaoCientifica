//https://randomnerdtutorials.com/esp32-web-server-arduino-ide/
#include <WiFi.h>


const char* ssid = "Wokwi-GUEST"; // <-- inserir wifi
const char* password = ""; // <-- inserir senha wifi


// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;



void setup() {
 
Serial.begin(115200);

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
                                  client.println("<!DOCTYPE html><html>");
                                  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                                  client.println("<link rel=\"icon\" href=\"data:,\">");
                                  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                                  client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                                  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                                  client.println(".button2 {background-color: #555555;}</style></head>");

                                  // Web Page Heading
                                  client.println("<body><h1>ESP32 Web Server</h1>");


                                  // Display current state, and ON/OFF buttons for GPIO 26  
                                  client.println("<p> paragrafo !!  </p>");

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

  // digitalWrite(LED, HIGH);
  // delay(500);
  // digitalWrite(LED, LOW);
  // delay(500);