#include "MPU6050.h"

void MPU6050::init() {
    uint8_t attempts = 0;
    Wire.setSDA(20); 
    Wire.setSCL(21); 
    if (!mpu.begin(0x68, &Wire, 0)) {
        Serial.println("Failed to find MPU6050 chip...");
        while (1)
            delay(10);
        Serial.print("Location attempt: ");
        Serial.println(attempts);
        Serial.println("Retrying to locate MPU6050 chip...");
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("");
    delay(100);
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