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
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    server.send(500, "text/plain", "Failed to read from DHT sensor");
    return;
  }

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="refresh" content="5">
<title>Cuarto</title>
<style>
  body {
    margin: 0;
    font-family: Arial, Helvetica, sans-serif;
    background: #e6e6e6; /* light grey background */
    color: #333333; /* dark text for contrast */
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
  }
  .card {
    background: #f5f5f5; /* slightly lighter grey card */
    border-radius: 16px;
    padding: 24px 32px;
    width: 300px;
    text-align: center;
    box-shadow: 0 10px 25px rgba(0,0,0,0.15);
  }
  h1 {
    margin: 0 0 16px;
    font-size: 1.4em;
  }
  .value {
    font-size: 2.5em;
    font-weight: bold;
    margin: 8px 0;
  }
  .label {
    font-size: 0.9em;
    opacity: 0.7;
  }
  .footer {
    margin-top: 16px;
    font-size: 0.75em;
    opacity: 0.6;
  }
</style>
</head>
<body>
  <div class="card">
    <h1>Cuarto</h1>

    <div class="label">Temperature</div>
    <div class="value">)rawliteral";

  html += String(temperature, 1);

  html += R"rawliteral( °C</div>

    <div class="label">Humidity</div>
    <div class="value">)rawliteral";

  html += String(humidity, 1);

  html += R"rawliteral( %</div>

    <div class="footer">Auto refresh every 5s</div>
  </div>
</body>
</html>
)rawliteral";

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
