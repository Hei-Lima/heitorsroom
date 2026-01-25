#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h> 

#define RGB_PIN 48
#define NUMPIXELS 1

// Inicializa o objeto do pixel
Adafruit_NeoPixel pixels(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define SDA 6 
#define SCL 5 

#define SCREEN_ADDRESS 0x3C 
#define OLED_RESET     -1 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 4
#define DHTTYPE DHT11

int currentId = 1;

DHT dht(DHTPIN, DHTTYPE);
const char* SSID = "xxx";
const char* PASSWORD = "xxx";
const char* ENVIRONMENT_POST = "http://xxxx:8000/environment";
const char* MESSAGE_URL_BASE = "http://xxxxx:8000/message/";
const char* API_TOKEN = "xxx"; 

struct MessageData {                                                                 
    String timestamp;
    int id;
    String message;
    String username;
};

int send_data(float h, float t, float hi);
MessageData get_messages(int id);

void setup() {
  Serial.begin(115200);
  dht.begin();

  pixels.begin();
  pixels.setBrightness(200); 
  pixels.clear(); 
  pixels.show();

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
  delay(10000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float hi = dht.computeHeatIndex(t, h, false);

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  send_data(h, t, hi);
  
  MessageData msg = get_messages(currentId);

  if (msg.id != 0 && msg.id != currentId - 1) {
    Serial.print("Msg: ");
    Serial.println(msg.message);

    notify_rgb(); 

    display_message(msg);
    currentId = currentId + 1;
  }

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

MessageData get_messages(int id) {
    MessageData data;
    data.id = 0; 

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(MESSAGE_URL_BASE) + String(id);
        http.begin(url);
        
        int httpResponseCode = http.GET();
        
        if (httpResponseCode > 0) {
            String payload = http.getString();
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, payload);

            if (!error) {
                data.timestamp = doc["timestamp"].as<String>();
                data.id = doc["id"];
                data.message = doc["message"].as<String>();
                data.username = doc["username"].as<String>();
            }
        }
        http.end();
    }
    return data;
}

void display_message(MessageData msg) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);

  display.println(msg.message);

  display.println("______________");

  display.print("User: ");
  display.println(msg.username);

  display.display();
}

void notify_rgb() {
  pixels.setPixelColor(0, pixels.Color(150, 0, 255)); 
  pixels.show(); 
  
  delay(1000);
  
  pixels.clear();
  pixels.show(); 
}