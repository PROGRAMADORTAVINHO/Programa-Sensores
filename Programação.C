#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>

#define button 33
#define sensor1 32
#define sensor2 35
#define sensor3 34

double t[4] = { };                                     
double distance[4] = { 0, 2.51, 5.06, 8.07 };                         
double averageSpeed[4] = { 0 };
double instantaneousSpeed[4] = { 0 };  
double averageAcceleration[4] = { 0 };                               

WebServer server(80);

const char ssid[] = "ssid";
const char password[] = "password";

String indexHtml(double averageSped[4], double instantaneousSpeed[4], double averageAcceleration[4]) {

  String htmlCode = "<!DOCTYPE html>\n";

  htmlCode += "<html lang=\"pt-br\">\n";
  htmlCode += "<head>\n";
  htmlCode += "<meta charset=\"UTF-8\">\n";
  htmlCode += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";

  htmlCode += "<title>Dados</title>\n";

  htmlCode += "<style>\n";
  htmlCode += "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  htmlCode += "body{margin-top: 50px;} \n";
  htmlCode += "h1 {color: #444444; margin: 50px auto 30px;}\n";
  htmlCode += "p {font-size: 24px; color: #444444; margin-bottom: 10px;}\n";
  htmlCode += "</style>\n";

  htmlCode += "</head>\n";
  htmlCode += "<body>\n";

  // Dados do sensor 1
  htmlCode += "<div id=\"dataSensor1\">\n";
  htmlCode += "<h1>Trecho 1</h1>\n";
  htmlCode += "<p>Velocidade média: ";
  htmlCode += averageSpeed[1];
  htmlCode += " m/s</p>\n";
  htmlCode += "<p>Velocidade instantânea no sensor: ";
  htmlCode += instantaneousSpeed[1];
  htmlCode += " m/s</p>\n";
  htmlCode += "<p>Aceleração média: ";
  htmlCode += averageAcceleration[1];
  htmlCode += " m/s</p>\n";
  htmlCode += "</div>\n";

  // Dados do sensor 2
  htmlCode += "<div id=\"dataSensor2\">\n";
  htmlCode += "<h1>Trecho 2</h1>\n";
  htmlCode += "<p>Velocidade média: ";
  htmlCode += averageSpeed[2];
  htmlCode += " m/s</p>\n";
  htmlCode += "<p>Velocidade instantânea no sensor: ";
  htmlCode += instantaneousSpeed[2];
  htmlCode += " m/s</p>\n";
  htmlCode += "<p>Aceleração média: ";
  htmlCode += averageAcceleration[2];
  htmlCode += " m/s</p>\n";
  htmlCode += "</div>\n";

  // Dados do sensor 3
  htmlCode += "<div id=\"dataSensor3\">\n";
  htmlCode += "<h1>Trecho 3</h1>\n";
  htmlCode += "<p>Velocidade média: ";
  htmlCode += averageSpeed[3];
  htmlCode += " m/s</p>\n";
  htmlCode += "<p>Velocidade instantânea no sensor: ";
  htmlCode += instantaneousSpeed[3];
  htmlCode += " m/s</p>\n";
  htmlCode += "<p>Aceleração média: ";
  htmlCode += averageAcceleration[3];
  htmlCode += " m/s</p>\n";
  htmlCode += "</div>\n";

  htmlCode += "</body>\n";
  htmlCode += "</html>\n";

  return htmlCode;

}

void handleRoot() {

  server.send(200, "text/html", indexHtml(averageSpeed, instantaneousSpeed, averageAcceleration));
}

void handleNotFound() {

  server.send(404, "text/plain", "Servidor não encontrado");
}

void setup() {

  pinMode(button, INPUT_PULLDOWN);
  pinMode(sensor1, INPUT_PULLDOWN);
  pinMode(sensor2, INPUT_PULLDOWN);
  pinMode(sensor3, INPUT_PULLDOWN);

  Serial.begin(115200);
  delay(5000);

  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("Servidor online");
}

void loop() {

  if (digitalRead(button) == HIGH) {
    t[0] = millis() / 1000;

    while (HIGH) {
      if (digitalRead(sensor1) == HIGH) {
        t[1] = millis() / 1000 - t[0];

      } else if (digitalRead(sensor2) == HIGH) {
        t[2] = millis() / 1000 - t[0];

      } else if (digitalRead(sensor3) == HIGH) {
        t[3] = millis() / 1000 - t[0];
        break;
      }
    }

    for (int i = 1; i < 4; i++) {
      averageSpeed[i] = (distance[i] - distance[i - 1]) / (t[i] - t[i - 1]);
      averageAcceleration[i] = 2 * (distance[i] - distance[i - 1] - instantaneousSpeed[i - 1] * (t[i] - t[i - 1])) / ((t[i] - t[i - 1]) * (t[i] - t[i - 1]));
      instantaneousSpeed[i] = instantaneousSpeed[i - 1] + averageAcceleration[i] * (t[i] - t[i - 1]);
    }
  }
  
  server.handleClient();

}