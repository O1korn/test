#include <WiFi.h>  // Бібліотека для роботи з Wi-Fi

const char* ssid = "TP-Link_7BF8";         // Ім'я Wi-Fi мережі
const char* password = "57444987";      // Пароль Wi-Fi мережі

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Підключення до Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Підключено до Wi-Fi");
  Serial.print("IP адреса: ");
  Serial.println(WiFi.localIP());         // Вивід IP-адреси після підключення
}

void loop() {
  // Основний код
}
