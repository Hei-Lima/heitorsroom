#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define SDA 6 // SDA pin
#define SCL 5 // SCL pin (DUH)

#define SCREEN_ADDRESS 0x3C 
#define OLED_RESET     -1 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
const char* SSID = "xxxxxx";
const char* PASSWORD = "xxxxx";
const char* ENVIRONMENT_POST = "http://xxxxx:8000/environment";
const char* API_TOKEN = "xxxxxx"; 

int send_data(float h, float t, float hi);

void setup() {
  Serial.begin(115200);
  dht.begin();

  Wire.begin(SDA, SCL); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.display();

  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi");
  
  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1); 
  display.println("WIFI CONNECTED!");
  display.println("");    
  display.println("IP Address:");
  display.setTextSize(1); 
  display.println(WiFi.localIP()); 
  display.display();
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
  int httpResponseCode = 0;

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(ENVIRONMENT_POST);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + String(API_TOKEN));

    String json = "{\"humidity\": " + String(h) + ", \"temperature\": " + String(t) + ", \"heatindex\": " + String(hi) + "}";
    
    httpResponseCode = http.POST(json);
    
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    http.end();
  }

  return httpResponseCode;
}