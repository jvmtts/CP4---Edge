#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
 
/*
Autores: Leonardo Borges da Costa, Enzo Amá fatobene, Paulo Henrique Barboza de Oliveira Ramos,João Victor Rodrigues Mattos, Pedro Henrique araújo de Abreu
Descrição: PoC Smart Lamp - ESP32 com DHT11 e LDR
Função: Ler temperatura, umidade e luminosidade e enviar para ThingSpeak
*/
 
// Conecta o ESP32 à rede Wi-Fi, usando as credenciais definidas abaixo
const char* ssid = "Wokwi-GUEST";
const char* password = "";
 
// Envia dados ao ThingSpeak; substitua pela sua chave de API e ID do canal
const char* apiKey = "ZXFXEIOY4I2DCJY4";
const char* channelID = "2413566";
 
// Lê o valor de luminosidade do LDR através deste pino analógico
const int ldrPin = 34; // Esses seguintes inteiros só representam os pinos para conexão
const int dhtPin = 2;
#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE); // Indicando o tipo de DHT
 
void setup() {
  Serial.begin(115200);
  connectWiFi(); // Chamamamos o método que conecta ao wifi
  dht.begin(); // Iniciamos o sensor DHT
}
 
void loop() {
  // Captura o valor atual do LDR para medir a intensidade da luz ambiente
  int ldrValue = analogRead(ldrPin);
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
 
  // Printa o valor do LDR
  Serial.print("Valor LDR: ");
  Serial.println(ldrValue);
 
  // Evia os dados ao thingSpeak
  dadosThingSpeak(ldrValue,temp,humi);
 
  // delay leve de 10 segundos
  delay(10000);
}
 
void connectWiFi() {
  Serial.println("Conectando ao WiFi");
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }
 
  Serial.println("Conectado ao WiFi");
}
 
void dadosThingSpeak(int value, int temperatura, int umidade) {
  HTTPClient http;
 
  String url = "http://api.thingspeak.com/update?api_key=" + String(apiKey) + "&field1=" + String(value) + "&field2=" + String(temperatura) + "&field3=" + String(umidade);
 
  Serial.println("Sending data to ThingSpeak...");
  Serial.println(url);
 
  if (http.begin(url)) {
    int httpResponseCode = http.GET();
    if (httpResponseCode == 200) {
      Serial.println("Data sent successfully to ThingSpeak!");
    } else {
      Serial.print("Error in sending data. HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Error in HTTP begin.");
  }
}
 