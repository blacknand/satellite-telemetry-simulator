#include <vector>
#include <Wire.h>
#include <SPI.h>

#include "bme280.h"
#include "MPU6050.h"
#include "I2C_functions.h"

BME280 bme280;
MPU6050 mpu6050;


void setup() {
    Serial.begin(115200);
    while (!Serial);                            // Wait for serial to initialise before continuing

    mpu6050.init();
    delay(1000);

    bme280.init();
}

void loop() {
    bme280.output_data();
    mpu6050.output_data();
    delay(10000);
}
