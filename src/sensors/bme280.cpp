#include "bme280.h"

#include "../data_preprocessing/json_conversion.h"

BME280Data bme_data;

void BME280::init() {
    Serial.println();
    Serial.println(F("Initialising BME280 sensor..."));

    bool status;
    
    status = bme.begin(0x77);  
    if (!status) {
        Serial.println("Could not locate BME280 sensor");
        while (1);
    }
    Serial.println("BME280 sensor located successfully");
    Serial.println();
}

BME280Data BME280::get_data() {
    BME280Data d;
    d.temperature = bme.readTemperature();
    d.pressure = bme.readPressure() / 100.0F;
    d.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    d.humidity = bme.readHumidity();
    return d;
}


