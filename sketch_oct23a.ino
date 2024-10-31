#include <Adafruit_SSD1331.h>

#include <OLED_I2C.h>   
#include <MQ135.h>
#include <DHT.h>

#define DHTPIN 4       // Пін для DHT датчика
#define DHTTYPE DHT22
#define PIN_MQ135 A2   // Пін для MQ135

OLED myOLED(A4, A5, 0x3C);  // Ініціалізація OLED дисплея з I2C адресою 0x3C (зазвичай стандартна)
extern uint8_t SmallFont[]; // Підключення латинського шрифту

MQ135 mq135_sensor(PIN_MQ135);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  myOLED.begin();
  myOLED.setFont(SmallFont); // Ініціалізуємо латинський шрифт
  myOLED.clrScr();           // Очищення екрану перед початком роботи
}

void loop() {
  delay(2000);

  // Зчитування температури і вологості
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Перевірка, чи є помилки при зчитуванні
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    myOLED.clrScr();
    myOLED.print("DHT Error!", CENTER, 0);
    myOLED.update();
    delay(2000);
    return;
  }

  // Показники MQ135
  float ppm = mq135_sensor.getPPM();

  // Виведення даних у серійний монітор
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.print(F("°C  "));
  Serial.print("MQ135 PPM: ");
  Serial.print(ppm);
  Serial.println(" ppm");

  // Виведення даних на OLED дисплей
  myOLED.clrScr();                                  // Очищення екрану перед оновленням
  myOLED.setFont(SmallFont);                        // Установка латинського шрифту

  myOLED.print("Temp:", LEFT, 0);                   // Виведення заголовку "Temp"
  myOLED.printNumF(temperature, 1, 50, 0);          // Виведення температури
  myOLED.print("C", RIGHT, 0);                      // Додавання символу °C

  myOLED.print("Humidity:", LEFT, 16);              // Виведення заголовку "Humidity"
  myOLED.printNumF(humidity, 1, 50, 16);            // Виведення вологості
  myOLED.print("%", RIGHT, 16);                     // Додавання символу %

  myOLED.print("Gas PPM:", LEFT, 32);               // Заголовок для концентрації газу
  myOLED.printNumF(ppm, 1, 50, 32);                 // Виведення концентрації газу в PPM

  myOLED.update();                                  // Оновлення дисплею для відображення змін
  delay(3000);                                      // Затримка перед наступним циклом
}
