#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// I2C RP2040 pins
#define SDA 18
#define SCL 19

class MPU6050 {
    private:
        Adafruit_MPU6050 mpu;
    public:
        void init();
        void output_data();
};