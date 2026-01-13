#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
const char* SSID = "NETOCO";
const char* PASSWORD = "a1b2c3d4";
const char* ENVIRONMENT_POST = "http://192.168.0.185:8000/environment";

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(15000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hi = dht.computeHeatIndex(t, h, false);

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  send_data(h, t, hi);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.print("°C  Heat Index: ");
  Serial.println(hi);
}

int send_data(float h, float t, float hi) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(ENVIRONMENT_POST);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"humidity\": " + String(h) + ", \"temperature\": " + String(t) + ", \"heatindex\": " + String(hi) + "}";
    
    int httpResponseCode = http.POST(json);
    
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    http.end();
  }

  return httpResponseCode
}