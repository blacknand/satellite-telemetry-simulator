#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mpu6050_i2c.h"
#include "bme280_i2c.h"
#include "data_preprocessing/utc_data.h"
#include "data_preprocessing/json_conversion.h"


using json = nlohmann::json;

BME280Data bme280_data;
MPU6050Data mpu6050_data;
SatelliteData satellite_data;
MPUDataStruct _mpu_data_struct;


int main() {

    stdio_init_all();
    sleep_ms(100);      // Allow device to stabalise

    mpu6050_init();
    bme280_init();

    while (true) {
        mpu6050_output();
        bme280_output();
        _mpu_data_struct = mpu6050_get_data();


        satellite_data.accel_x = _mpu_data_struct.acceleration[0];
        satellite_data.accel_y = _mpu_data_struct.acceleration[1];
        satellite_data.accel_z = _mpu_data_struct.acceleration[2];
        satellite_data.gyro_x = _mpu_data_struct.gyro[1];
        satellite_data.gyro_y = _mpu_data_struct.gyro[2];
        satellite_data.gyro_z = _mpu_data_struct.gyro[3];
        
        sleep_ms(1000);
    }
    return 0;    
}
