#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class MPU6050 {
    private:
        Adafruit_MPU6050 mpu;
    public:
        void init();
        void output_data();
};