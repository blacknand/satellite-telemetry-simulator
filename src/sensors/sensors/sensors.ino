/*
  > Integration of both the BME280 and MPU6050 sensors to output in a single sketch
  > relays the raw data from both sensors
  > author: nblackburndeveloper@icloud.com; github@blacknand
*/

#include <vector>
#include <Wire.h>
#include <SPI.h>

#include "bme280.h"
#include "MPU6050.h"
#include "I2C_scanner.h"

BME280 bme280;
MPU6050 mpu6050;


void setup() {
    Serial.begin(115200);
    while (!Serial);                            // Wait for serial to initialise before continuing

    // Serial.println("Initialising BME280...");
    // // bme280.init();
    // Serial.println("Initialised BME280 sensor...");
    // delay(500); 
    // Serial.println("Initialising MPU6050 sensor...");
    // mpu6050.init();
    // Serial.println("Initialised MPU6050 sensor...");
    Wire.begin();
    mpu6050.get_wire().begin();
    Serial.println("Scanning default Wire...");    
    i2c_scanner(Wire, "Wire (Default I2C)");
    Serial.println("Scanning custom Wire...");
    i2c_scanner(mpu6050.get_wire(), "Wire (Custom I2C)");
}

void loop() {
    // bme280.output_data();
    // mpu6050.output_data();
    Serial.println("loop");
    delay(10000);
}
