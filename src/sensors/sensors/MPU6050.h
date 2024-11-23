#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class MPU6050 {
    private:
        Adafruit_MPU6050 mpu;
        TwoWire WireMPU;
    public:
        MPU6050();
        void init();
        void output_data();
        TwoWire& get_wire() { return WireMPU; }
};