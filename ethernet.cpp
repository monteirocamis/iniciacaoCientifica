// Projeto 46 – Baseado no exemplo Arduino Webserver, criado por David A. Mellis e Tom Igoe
#include <SPI.h>
#include <Ethernet.h> 
#include <OneWire.h>
#include <DallasTemperature.h>

// Fio de dados é plugado ao pino 3 do Arduino 
#define ONE_WIRE_BUS 3
#define TEMPERATURE_PRECISION 12

float tempC, tempF;
// Prepara uma instância de OneWire para se comunicar com qualquer dispositivo 1-Wire (não // apenas com CIs de temperatura Maxim/Dallas)
OneWire oneWire(ONE_WIRE_BUS);

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

    // Inicia a ethernet e o servidor
    Ethernet.begin(mac, ip);
    server.begin();

    // Inicaliza a biblioteca sensors
    sensors.begin();

    // define a resolução
    sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION); 
    sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);
}

// função para obter a temperatura de um dispositivo
 void getTemperature(DeviceAddress deviceAddress) {
    tempC = sensors.getTempC(deviceAddress);
    tempF = DallasTemperature::toFahrenheit(tempC); 
}

void loop() { sensors.requestTemperatures();

// escuta a entrada de clientes 
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

    // Permite um intervalo de tempo suficiente para que o navegador receba os dados 
    delay(10);
    // Fecha a conexão
    client.stop();
    } 
}



// abrir o navegador web no IP e a porta exemplo: 192.168.0.104:80
