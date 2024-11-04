// C++ header files
#include <vector>

// BME280 sensor header files
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Local files
#include "bme280.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup() {
    Serial.begin(9600);
    while(!Serial);    
    Serial.println(F("BME280 initialisation"));

    unsigned status;
    
    status = bme.begin();       // In case of sensor error, try address 0x77 (uint8_t)
    uint8_t tries = 0;                                            
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        Serial.print("Location attempt: ");
        Serial.println(tries);
        Serial.print("Retying to locate BME280 sensor...");
        tries++;
        while (1) delay(10);
    }

    Serial.println();
}


void loop() { 
    printValues();
}


void printValues() {
    /*
      * bme.readTemperature()           float
      * bme.readPressure()              float
      * bme.readAltitude()              float
      * bme.readHumidity()              float
    */
    std::vector<float> bme_280_results = bme280_values();
    for (const int& i: bme_280_results) {
            Serial.print("Temperature: ");
            Serial.print(bme.readTemperature());      
            Serial.println("°C");

            Serial.print("Pressure: ");

            Serial.print(bme.readPressure() / 100.0F);
            Serial.println(" hPa");

            Serial.print("Approx. Altitude: ");
            Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
            Serial.println(" m");

            Serial.print("Humidity: ");
            Serial.print(bme.readHumidity());
            Serial.println("%");

            Serial.println();
    }
}

std::vector<float> bme280_values() {
    std::vector<float> bme280_vect;
    bme280_vect.push_back(bme.readTemperature());
    bme280_vect.push_back(bme.readPressure());
    bme280_vect.push_back(bme.readAltitude(SEALEVELPRESSURE_HPA));
    bme280_vect.push_back(bme.readHumidity());
    return bme280_vect;
}