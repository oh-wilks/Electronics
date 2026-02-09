#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include "secrets.h"

#define DHTPIN 4        // GPIO4 (D2)
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);


// ---------- HANDLER ----------
void handleRoot() {
  float temperature = dht.readTemperature(); // Celsius
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    server.send(500, "text/plain", "Failed to read from DHT sensor");
    return;
  }

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<title>ESP8266 DHT22</title></head><body>";
  html += "<h1>DHT22 Sensor Data</h1>";
  html += "<p><b>Temperature:</b> " + String(temperature) + " &deg;C</p>";
  html += "<p><b>Humidity:</b> " + String(humidity) + " %</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println();
  Serial.println("Starting ESP8266...");
  
  dht.begin();

  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(500);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - startAttemptTime < 15000) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("WiFi status: ");
  Serial.println(WiFi.status());

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("❌ WiFi connection failed");
  }

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

// ---------- LOOP ----------
void loop() {
  server.handleClient();
}
