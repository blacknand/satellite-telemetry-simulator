#ifndef BME280_SENSOR_H
#define BME280_SENSOR_H

#include <Adafruit_BME280.h>
#include <vector>

class BME280 {
    private:
        Adafruit_BME280 bme;
        const float sea_level_pressure_hpa = 1008.5;          // https://meteologix.com/uk/model-charts/euro/united-kingdom/sea-level-pressure.html; specifically for east and north east of England 
    public:
        void init();
        void output_data();
        std::vector<float> bme280_values();
};

#endif      // BME_280_SENSOR_H