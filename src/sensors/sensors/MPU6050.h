#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


#include "../json.hpp"

// I2C RP2040 pins
#define SDA 18
#define SCL 19

using json = nhlomann::json;

#ifndef MPU6050_H
#define MPU6050_H

struct MPU6050Data {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
};

extern MPU6050Data mpu_data;

class MPU6050 {
    private:
        Adafruit_MPU6050 mpu;

    public:
        void init();
        void output_data();
        friend void to_json(json& j, const _data& d);
        friend void from_json(json& j, const _data& d);
};

#endif          // MPU6050