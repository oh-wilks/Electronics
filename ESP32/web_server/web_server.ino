#include <WiFi.h>
#include <WebServer.h>
#include "secrets.h"

WebServer server(80);

void handleRoot() {
  server.send(200, "text/html",
              "<h1>ESP32 Web Server</h1>"
              "<p>This is a barebones page served by ESP32.</p>");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Connecting to WiFi...");
  WiFi.disconnect(true);
  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  unsigned long startAttemptTime = millis();


  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
    Serial.print(".");
    delay(500);
  }


  Serial.println();
  Serial.print("Status: ");
  Serial.println(WiFi.status());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}