#include "Wire.h"
#include "Seeed_BME280.h"
#include "SeeedGrayOLED.h"
#include "EEPROM.h"

#define oled SeeedGrayOled

struct bme280_s
{
  uint32_t pressure;
  float temperature;
  float altitude;
  uint32_t humidity;
};

BME280 bme280;

int count = 0;

void setup() {
  uint8_t start_count;
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Initializing...");
  Wire.begin();
  // BME 280
  if (!bme280.init()) 
  {
    Serial.println("Device error BME280");
  }
  // OLED
  SeeedGrayOled.init(SH1107G);
  SeeedGrayOled.setNormalDisplay();
  SeeedGrayOled.clearDisplay();
  SeeedGrayOled.setVerticalMode();
  pinMode(LED_BUILTIN, OUTPUT);
  EEPROM.get(1023, start_count);
  start_count++;
  EEPROM.write(1023, start_count);
}

void loop() {
  uint8_t start_count;
  led_on(500);
  int y_offset = 7;

  struct bme280_s bme280_data = bme280_get_all();
  oled.setTextXY(0, 0);
  oled.putString("Barometer");
 
  oled.setTextXY(1, 0);
  oled.putString("Pres");
  oled.setTextXY(1, y_offset);
  oled.putNumber(bme280_data.pressure/100);
  oled.putString("hPa");
  
  oled.setTextXY(2, 0);
  oled.putString("Temp");
  oled.setTextXY(2, y_offset);
  oled.putNumber(bme280_data.temperature);
  oled.putString("C");

  oled.setTextXY(3, 0);
  oled.putString("Hum");
  oled.setTextXY(3, y_offset);
  oled.putNumber(bme280_data.humidity);
  oled.putString("%RH");

  oled.setTextXY(4, 0);
  oled.putString("Alt");
  oled.setTextXY(4, y_offset);
  oled.putNumber(bme280_data.altitude);
  oled.putString("m");

  oled.setTextXY(15, 0);
  oled.putString("Boot# ");
  EEPROM.get(1023, start_count);
  oled.putNumber(start_count);
  
  count++;
  led_off(1000);
}

struct bme280_s bme280_get_all()
{
  struct bme280_s bme280_data;
  bme280_data.pressure = bme280.getPressure();
  bme280_data.temperature = bme280.getTemperature();
  bme280_data.altitude = bme280.calcAltitude(bme280_data.pressure);
  bme280_data.humidity = bme280.getHumidity();
  return bme280_data;
}

void led_on(int sleep)
{
  digitalWrite(LED_BUILTIN, HIGH);
  // Serial.println("led on");
  delay(sleep); 
}

void led_off(int sleep)
{
  digitalWrite(LED_BUILTIN, LOW);
  // Serial.println("led off");
  delay(sleep);
 }
