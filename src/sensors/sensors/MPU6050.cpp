#include "MPU6050.h"
#include "I2C_functions.h"


void MPU6050::init() {
    int retries = 5; 
    bool success = false;
    while (retries > 0 && !success) {
        if (mpu.begin(0x68, &Wire)) {
            Serial.println("MPU6050 sensor initialized\n");
            success = true;
            mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
            mpu.setGyroRange(MPU6050_RANGE_250_DEG);
            mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
            Serial.println("MPU6050 sensor initialised");
            delay(100);
        } else {
            Serial.println("Retrying MPU6050 initialization...");
            delay(500); 
        }
        retries--;
    }
    if (!success) {
        Serial.println("Failed to initialize MPU6050 after retries");
        while (1) delay(10);            // Halt if it still fails
    }
}


void MPU6050::output_data() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    Serial.print("AccelX:");
    Serial.print(a.acceleration.x);
    Serial.print(",");
    Serial.print("AccelY:");
    Serial.print(a.acceleration.y);
    Serial.print(",");
    Serial.print("AccelZ:");
    Serial.print(a.acceleration.z);
    Serial.print(", ");
    Serial.print("GyroX:");
    Serial.print(g.gyro.x);
    Serial.print(",");
    Serial.print("GyroY:");
    Serial.print(g.gyro.y);
    Serial.print(",");
    Serial.print("GyroZ:");
    Serial.print(g.gyro.z);
    Serial.println("");

    delay(10);
}