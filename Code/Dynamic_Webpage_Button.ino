#include "index.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const uint8_t LED_Pin = 2;

const char* ssid = "HIPAA";
const char* password = "123456789";
const char* http_username = "user";
const char* http_password = "123456789";

boolean LED_Status;
const char* LED_Param = "LED";

AsyncWebServer server(80);

IPAddress local_IP(192, 168, 137, 77);
IPAddress gateway(10, 32, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

void setup() {
  Serial.begin(115200);
  pinMode(LED_Pin, OUTPUT);
  digitalWrite(LED_Pin, LOW);

  WiFi_Init();
  Server_Init();
}

void loop() {
}

String processor(const String& var) {
  if (var == "LED_Status") {
    if (LED_Status)return "LED ON";
    else return "LED OFF";
  }
  return String();
}

void WiFi_Init() {
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    Serial.println("STA Failed to configure.");
  else Serial.println("STA Succeeded to config.");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }
  Serial.println();
  Serial.println(WiFi.localIP());
}

void Server_Init() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/LED", HTTP_GET, [](AsyncWebServerRequest * request) {
    String inputMessage = "OK";
    if (request->hasParam(LED_Param)) {
      inputMessage = request->getParam(LED_Param)->value();
      if (inputMessage == "true")LED_Status = true;
      else LED_Status = false;
      if (LED_Status) {
        digitalWrite(LED_Pin, HIGH);
        inputMessage = "LED ON";
      }
      else {
        digitalWrite(LED_Pin, LOW);
        inputMessage = "LED OFF";
      }
    }
    else inputMessage = "Transmit failed!";
    Serial.println(inputMessage);
    request->send(200, "text/plain", inputMessage);
  });
  server.begin();
}
