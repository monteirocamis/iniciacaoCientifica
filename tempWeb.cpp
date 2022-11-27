 //projeto 36 no thinkercad + wearable femRGB c/ nodemcu + youtube thingspeak + projeto 46 web

#include <ESP8266WiFi.h>
 #include <SPI.h>
//#include <Ethernet.h> 
#include <OneWire.h>
//#include <DallasTemperature.h>

const char* ssid = "..."; // <-- inserir wifi
const char* password = "..."; // <-- inserir senha wifi
 
 // thingspeak
String host     = "api.thingspeak.com"; // Open Weather Map API
const int httpPort   = 80;
String uri     = "/update?api_key=5SXNU1Q3CXE33SAD&field1=";


WiFiServer server(80);

#define sensorPin 0
#define ONE_WIRE_BUS 3// Fio de dados é plugado ao pino 3 do Arduino 
#define TEMPERATURE_PRECISION 12

//float tempC, tempF;
// Prepara uma instância de OneWire para se comunicar com qualquer dispositivo 1-Wire (não // apenas com CIs de temperatura Maxim/Dallas)
OneWire oneWire(ONE_WIRE_BUS);

float Celsius, Fahrenheit, Kelvin;
int sensorValue;

// Passa o endereço da instância oneWire ao Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays para armazenar os endereços dos dispositivos
DeviceAddress insideThermometer = { 0x10, 0x7A, 0x3B, 0xA9, 0x01, 0x08, 0x00, 0xBF };
DeviceAddress outsideThermometer = { 0x10, 0xCD, 0x39, 0xA9, 0x01, 0x08, 0x00, 0xBE};
byte mac[] = { 0x48, 0xC2, 0xA1, 0xF3, 0x8D, 0xB7 };
byte ip[] = { 192, 168 ,0, 104 };

// O endereço MAC (Media Access Control) é um identificador único para interfaces de rede. A placa de rede em seu PC ou Mac terá seu endereço MAC definido pelo fabricante. No seu caso, você mesmo está decidindo qual deve ser esse endereço. Trata-se simplesmente de um número de 48 bits, por isso utilize quaisquer seis dígi- tos hexadecimais no campo do endereço, se bem que não há problema em deixá-lo como está. O endereço IP terá de ser manualmente definido, e deve estar dentro do intervalo permitido pelo seu roteador.

// Inicia o servidor na porta 80 
Server server(80);

void setup() {
    Serial.begin(9600); // inicia serial em 9600
    Serial.println("Inicio....."); // mostra texto no serial
    delay(10);

    // Inicia a ethernet e o servidor
    // Ethernet.begin(mac, ip);
    // server.begin();

        
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




    // Inicaliza a biblioteca sensors
    //sensors.begin();

    // define a resolução
   // sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION); 
   // sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);

 
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


     // sensors.requestTemperatures();

// web - entrada clientes
Client client = server.available(); 
if (client) {
    // uma solicitação http termina com uma linha em branco 
    boolean BlankLine = true;
    while (client.connected()) {
        if (client.available()) {
             char c = client.read();

        // Se a linha estiver em branco e o fim da linha for um caractere newline '\n' = fim da // solicitação HTTP
            if (c == '\n' && BlankLine) {
                getTemperature(insideThermometer);
                client.println("HTTP/1.1 200 OK"); // Resposta HTTP padrão 
                client.println("Content-Type: text/html\n"); 
                client.println("<html><head><META HTTP-EQUIV=""refresh""CONTENT=""5"">\n"); 
                client.println("<title>Arduino Web Server</title></head>"); 
                client.println("<body>\n");
                client.println("<h1>Arduino Web Server</h1>"); 
                client.println("<h3>Internal Temperature</h3>");
                client.println("Temp C:");
                client.println(tempC);
                client.println("<br/>");
                client.println("Temp F:");
                client.println(tempF);
                client.println("<br/>");
                getTemperature(outsideThermometer);
                client.println("<h3>External Temperature</h3>");
                client.println("Temp C:");
                client.println(tempC); 
                client.println("<br/>");
                client.println("Temp F:");
                client.println(tempF); 
                client.println("<br/>");
                client.println("<br/>");
                client.println("<p> with ♥︎ by Camis - 2022 </p>");
             break; 
            }

        if (c == '\n') {
        // Iniciando uma nova linha 
            BlankLine = true;
        }
        else if (c != '\r') {
        // A linha atual tem um caractere nela
         BlankLine = false; }
        } 
    }
}

    // intervalo p/ navegador receba os dados 
    delay(10);
    // fecha conexão
    client.stop();
    } 

}