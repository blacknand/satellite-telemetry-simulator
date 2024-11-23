#include <vector>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "bme280.h"

Adafruit_BME280 bme; // I2C

void BME280::init() {
    unsigned status;
    Wire.begin();
    status = bme.begin(0x77, &Wire);       // In case of sensor error, try address 0x77 (uint8_t)
    if (!status) {
        Serial.println("Could not find a locate BME280 sensor...");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        while (1) delay(10);
    }

    Serial.println("BME280 initialised");   
}


void BME280::output_data() {
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
            Serial.print(bme.readAltitude(sea_level_pressure_hpa));
            Serial.println(" m");

            Serial.print("Humidity: ");
            Serial.print(bme.readHumidity());
            Serial.println("%");

            Serial.println();
    }
}


std::vector<float> BME280::bme280_values() {
    std::vector<float> bme280_vect;
    bme280_vect.push_back(bme.readTemperature());
    bme280_vect.push_back(bme.readPressure());
    bme280_vect.push_back(bme.readAltitude(sea_level_pressure_hpa));
    bme280_vect.push_back(bme.readHumidity());
    return bme280_vect;
}