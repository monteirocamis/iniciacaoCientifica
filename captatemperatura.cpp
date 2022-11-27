// Projeto 36 
#define sensorPin 0
float Celsius, Fahrenheit, Kelvin;
int sensorValue;
void setup() {
    Serial.begin(9600); // inicia serial em 9600
    Serial.println("Initialising....."); // mostra texto no serial
}
void loop() {
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