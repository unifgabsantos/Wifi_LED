#include <WiFi.h>
#define pinVermelho 5
#define pinVerde 18
#define pinAzul 19
const char* ssid = "";
const char* password =  "";
WiFiServer server(80);
void Vermelho() {
  digitalWrite(pinVermelho, 1);
  digitalWrite(pinVerde, 0);
  digitalWrite(pinAzul, 0);
};
void Verde() {
  digitalWrite(pinVermelho, 0);
  digitalWrite(pinVerde, 1);
  digitalWrite(pinAzul, 0);
};
void Azul() {
  digitalWrite(pinVermelho, 0);
  digitalWrite(pinVerde, 0);
  digitalWrite(pinAzul, 1);
};

void Branco() {
  digitalWrite(pinVermelho, 1);
  digitalWrite(pinVerde, 1);
  digitalWrite(pinAzul, 1);
};
void Desligar(){
  digitalWrite(pinVermelho, 0);
  digitalWrite(pinVerde, 0);
  digitalWrite(pinAzul, 0);
}
const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Led</title>
</head>
<body style="text-align: center;">
    <h1><a href='/Vermelho'>Vermelho</a></h1>
    <h1><a href='/Verde'>Verde</a></h1>
    <h1><a href='/Azul'>Azul</a></h1>
    <h1><a href='/Branco'>Branco</a></h1>
    <h1><a href='/Desligar'>Desligar</a></h1>
</body>
</html>

)=====";
void Conectar(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
};
void setup(){
  Serial.begin(115200);
  Conectar();
  pinMode(pinVermelho,OUTPUT);
  pinMode(pinVerde,OUTPUT);
  pinMode(pinAzul,OUTPUT);
}
void loop() {
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print(webpage);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /Vermelho")) Vermelho();
        else if (currentLine.endsWith("GET /Verde")) Verde();
        else if (currentLine.endsWith("GET /Azul")) Azul();
        else if (currentLine.endsWith("GET /Branco")) Branco();
        else if (currentLine.endsWith("GET /Desligar")) Desligar();
      }
    }
    client.stop();
  }
}
