#include <stdio.h>

#include "satellite_telemetry_simulator.h"
#include "../common/satellite_data.h"


json j;
BME280Data bme280_data;
MPU6050Data mpu6050_data;
SatelliteData satellite_data;
MPUDataStruct _mpu_data_struct;


json get_satellite_data() {
    _mpu_data_struct = mpu6050_get_data();
    bme280_data = get_bme_data();
    SatelliteData satellite_data;
    
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

    json _j = satellite_data;
    return _j;
}


// main is only executed on the Pico
int main() {

    stdio_init_all();
    sleep_ms(100);      // Allow device to stabalise

    mpu6050_init();
    bme280_init();

    while (true) {
        j = get_satellite_data();
        printf("%s\n", j.dump(4).c_str());
        
        sleep_ms(1000);
    }
    return 0;    
}
