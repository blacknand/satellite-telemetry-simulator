#include "bme280.h"


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
