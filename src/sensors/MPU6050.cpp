#include <fstream>
#include "MPU6050.h"
#include "bme280.h"
#include "I2C_functions.h"

#include "/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/data_preprocessing/json_conversion.h"

MPU6050Data mpu_data;

void MPU6050::init() {
    Serial.println("Initialising MPU6050 sensor...");
    Wire1.setSDA(SDA);
    Wire1.setSCL(SCL);
    Wire1.begin();

    // Initialise
    if (!mpu.begin(0x68, &Wire1, 0)) {
        Serial.println("Failed to locate MPU6050 sensor");
        while (1) { delay(10); }
    }
    Serial.println("Successfully located MPU6050 sensor");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange()) {
        case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
        case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
        case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
        case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }

    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange()) {
        case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
        case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
        case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
        case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth()) {
        case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
        case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
        case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
        case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
        case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
        case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
        case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }
}


void MPU6050::output_data() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    mpu_data.accel_x = a.acceleration.x;
    mpu_data.accel_y = a.acceleration.y;
    mpu_data.accel_z = a.acceleration.z;
    mpu_data.gyro_x = g.gyro.x;
    mpu_data.gyro_y = g.gyro.y;
    mpu_data.gyro_z = g.gyro.z;
    json j = mpu_data;
    Serial.println(j.dump().c_str());
    Serial.println();
    // delay(1000);
}


// void to_json(json& j, MPU6050Data& d) {
//     j = json {
//         {"accel_x", d.accel_x},
//         {"accel_y", d.accel_y},
//         {"accel_z", d.accel_z},
//         {"gyro_x", d.gyro_x},
//         {"gyro_y", d.gyro_y},
//         {"gyro_z", d.gyro_z}
//     };
// }


// void from_json(json& j, MPU6050Data& d) {
//     j.at("accel_x").get_to(d.accel_x);
//     j.at("accel_y").get_to(d.accel_y);
//     j.at("accel_z").get_to(d.accel_z);
//     j.at("gyro_x").get_to(d.gyro_x);
//     j.at("gyro_y").get_to(d.gyro_y);
//     j.at("gyro_z").get_to(d.gyro_z);
// }