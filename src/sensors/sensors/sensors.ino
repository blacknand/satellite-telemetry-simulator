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

    bme280.init();
    i2c_scanner(Wire, "default");

    // Delay and reset I2C bus
    delay(1000);
    reset_i2c_bus();
    delay(1000);

    // Initialize MPU6050
    // Wire.setSDA(20);  // Set pins for MPU6050
    // Wire.setSCL(21);
    // mpu6050.init();

    i2c_scanner(Wire, "default after MPU6050 init");

  }


void loop() {
    bme280.output_data();
    mpu6050.output_data();
    delay(10000);
}
