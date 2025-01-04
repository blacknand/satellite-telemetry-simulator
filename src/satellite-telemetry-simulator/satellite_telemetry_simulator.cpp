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
        _mpu_data_struct = mpu6050_get_data();
        bme280_data = get_bme_data();
        
        satellite_data.accel_x = _mpu_data_struct.acceleration[0];
        satellite_data.accel_y = _mpu_data_struct.acceleration[1];
        satellite_data.accel_z = _mpu_data_struct.acceleration[2];
        satellite_data.gyro_x = _mpu_data_struct.gyro[1];
        satellite_data.gyro_y = _mpu_data_struct.gyro[2];
        satellite_data.gyro_z = _mpu_data_struct.gyro[3];

        satellite_data.temperature = bme280_data.temperature;
        satellite_data.pressure = bme280_data.pressure;
        satellite_data.humidity = bme280_data.humidity;
        satellite_data.altitude = bme280_data.altitude;

        satellite_data.utc_time = get_utc_time();
        satellite_data.mpu_temp = _mpu_data_struct.temp;

        json j = satellite_data;
        printf("%s\n", j.dump(4).c_str());
        
        sleep_ms(1000);
    }
    return 0;    
}
