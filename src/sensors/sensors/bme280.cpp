#include "bme280.h"

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


void BME280::loop_output() {
    output_data();
    // delay(1000);
}


void BME280::output_data() {

    bme_data.temperature = bme.readTemperature();
    bme_data.pressure = bme.readPressure() / 100.0F;
    bme_data.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    bme_data.humidity = bme.readHumidity();

    Serial.print("Temperature = ");
    Serial.print(bme_data.temperature);
    Serial.println(" *C"); 
    
    Serial.print("Pressure = ");
    Serial.print(bme_data.pressure);
    Serial.println(" hPa");
    
    Serial.print("Approx. Altitude = ");
    Serial.print(bme_data.altitude);
    Serial.println(" m");
    
    Serial.print("Humidity = ");
    Serial.print(bme_data.humidity);
    Serial.println(" %");
    
    Serial.println();
}
