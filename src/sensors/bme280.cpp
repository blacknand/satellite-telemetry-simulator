#include "bme280.h"

#include "/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/data_preprocessing/json_conversion.cpp"

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


// void BME280::loop_output() {
    // output_data();
    // // delay(1000);
// }


BME280Data BME280::get_data() {
    BME280Data d;
    d.temperature = bme.readTemperature();
    d.pressure = bme.readPressure() / 100.0F;
    d.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    d.humidity = bme.readHumidity();
    return d;
}


// void to_json(json& j, BME280Data& d) {
    // j = json {
        // {"temperature (*C)", d.temperature},
        // {"pressure (hPa)", d.pressure},
        // {"altitude (m)", d.altitude},
        // {"humidity (%)", d.humidity}
    // };
// }


// void from_json(json& j, BME280Data& d) {
    // j.at("temperature").get_to(d.temperature);
    // j.at("pressure").get_to(d.pressure);
    // j.at("altitude").get_to(d.altitude);
    // j.at("humidity").get_to(d.humidity);
// }