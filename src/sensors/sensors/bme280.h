#ifndef BME280_SENSOR_H
#define BME280_SENSOR_H

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/json.hpp"


#define SEALEVELPRESSURE_HPA (1013.25)
using json = nlohmann::json;

struct BME280Data {
    float temperature;
    float pressure;
    float altitude;
    float humidity;
};

extern BME280Data bme_data;

class BME280 {
    private:
        Adafruit_BME280 bme;
    public:
        void init();
        void output_data();
        void loop_output();
        void to_json(json& j, BME280Data& d);
        void from_json(json& j, BME280Data& d);
};

#endif      // BME_280_SENSOR_H