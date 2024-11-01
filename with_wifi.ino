#include <Adafruit_SSD1331.h>
#include <OLED_I2C.h>
#include <MQ135.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DefaultFonts.h"  // Змініть назву на правильну, якщо потрібно


#define DHTPIN 4
#define DHTTYPE DHT22
#define PIN_MQ135 A2

OLED myOLED(A4, A5, 0x3C);
extern uint8_t SmallFont[];

MQ135 mq135_sensor(PIN_MQ135);
DHT dht(DHTPIN, DHTTYPE);

// Wi-Fi налаштування
const char* ssid = "TP-Link_7BF8";
const char* password = "57444987";
const char* serverUrl = "http://192.168.1.104:5000/data";  // Замість YOUR_PC_IP_ADDRESS вкажіть IP вашого ПК

void setup() {
  Serial.begin(9600);
  dht.begin();
  myOLED.begin();
  myOLED.setFont(SmallFont);
  myOLED.clrScr();

  // Підключення до Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    myOLED.clrScr();
    myOLED.print("DHT Error!", CENTER, 0);
    myOLED.update();
    delay(2000);
    return;
  }

  float ppm = mq135_sensor.getPPM();

  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.print(F("°C  "));
  Serial.print("MQ135 PPM: ");
  Serial.print(ppm);
  Serial.println(" ppm");

  myOLED.clrScr();
  myOLED.setFont(SmallFont);
  myOLED.print("Temp:", LEFT, 0);
  myOLED.printNumF(temperature, 1, 50, 0);
  myOLED.print("C", RIGHT, 0);
  myOLED.print("Humidity:", LEFT, 16);
  myOLED.printNumF(humidity, 1, 50, 16);
  myOLED.print("%", RIGHT, 16);
  myOLED.print("Gas PPM:", LEFT, 32);
  myOLED.printNumF(ppm, 1, 50, 32);
  myOLED.update();

  sendDataToServer(temperature, humidity, ppm);  // Відправка даних на сервер
  delay(3000);
}

void sendDataToServer(float temperature, float humidity, float ppm) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"temperature\":" + String(temperature) +
                      ",\"humidity\":" + String(humidity) +
                      ",\"ppm\":" + String(ppm) + "}";

    int httpResponseCode = http.POST(jsonData);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Data sent to server: " + response);
    } else {
      Serial.println(String("Error in sending data: ") + http.errorToString(httpResponseCode).c_str());

    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
