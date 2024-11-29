// BME280 sensor

// ESP8266 nodeMCU v3 GPIO5 D1 SCL GPIO4 D2 SDA
// ESP32 devkit GPIO22 SCL GPIO21 SDA
// RP2040 devkit GPIO25 SCL GPIO4 SDA

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "bme280_test.h"


#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup_BME280() {
  Serial.begin(115200);
  delay(2000);
  Serial.println();
  Serial.println(F("BME280 test"));

  bool status;
 
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x77);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  Serial.println("BME280 found -- Default Test --");
  delayTime = 1000;
  Serial.println();
}


void loop_BME_280() { 
  printValues();
  //delay(delayTime);
}


void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");
  
  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
  
  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
  
  Serial.println();
}