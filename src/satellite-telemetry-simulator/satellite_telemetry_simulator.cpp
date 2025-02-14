#include <stdio.h>
#include <atomic>
#include <chrono>
// #include <thread>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/mutex.h"         // ?

#include "satellite_telemetry_simulator.h"
#include "set_telemetry.h"
#include "../common/satellite_data.h"


json j;
BME280Data bme280_data;
MPU6050Data mpu6050_data;
SatelliteData satellite_data;
MPUDataStruct _mpu_data_struct;
bool calibrated;

// Global atomic flag to check if collect_telemetry function needs to be terminated
std::atomic<bool> stop_telemetry = false;
std::atomic<int> telemetry_delay = 1000;            // Initial value, 1 full second

json get_satellite_data() {
    _mpu_data_struct = mpu6050_get_data();
    bme280_data = get_bme_data();
    SatelliteData satellite_data;
   
    // Get the satellite data and assign to the satellite_data struct
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

    // Construct the JSON object with the satellite data
    json _j = {
        {"utc_data", {
            {"UTC time", satellite_data.utc_time}
        }},
        {"sensor_data", {
            {"accelerometer", {
                {"accel_x", satellite_data.accel_x},
                {"accel_y", satellite_data.accel_y},
                {"accel_z", satellite_data.accel_z}
            }},
            {"gyroscope", {
                {"gyro_x", satellite_data.gyro_x},
                {"gyro_y", satellite_data.gyro_y},
                {"gyro_z", satellite_data.gyro_z}
            }},
            {"environment", {
                {"altitude (m)", satellite_data.altitude},
                {"humidity (%)", satellite_data.humidity},
                {"pressure (hPa)", satellite_data.pressure},
                {"temperature (*C)", satellite_data.temperature}
            }},
            {"sensor_meta_data", {
                {"mpu_temperature", satellite_data.mpu_temp}
            }}
        }}
    };

    return _j;
}


void collect_telemetry() {
    while (true) {
        // Change the rate at which telemetry data is collected 
        if (!stop_telemetry.load()) {
            j = get_satellite_data();
            printf("START OF JSON OBJECT\n");
            printf("%s\n", j.dump(4).c_str());
            printf("END OF JSON OBJECT\n"); 
            sleep_ms(telemetry_delay.load());   // Use the dynamic delay
        }  else {
            // If stopped just yield idle CPU time at 100 ms 
            sleep_ms(100); 
        }
    }    
}


void set_telemetry_delay(int new_delay) {
    // stop_telemetry.store()
    telemetry_delay.store(new_delay);
}


// main is only executed on the Pico
int main() {
    calibrated = false;
    stdio_init_all();
    sleep_ms(100);      // Allow device to stabalise

    mpu6050_init();
    bme280_init();
    calibrated = true;
    j = get_satellite_data();

    /**
     * The Pico W has 2 cores, core 0 and core 1. Obviously core 0 is used for the main thread of execution
     * and core 1 can be started using multicore_launch_core1, once core 1 has been started it cannot be terminated
     * like normal threads can be so it will be forever running, because of this instead we need to use the atomic value
     * to dynamically change the value of the atomic value on the main thread during program execution and then constantly
     * check it on core 1 to see if the telemetry_collection process needs to stop at any point
     * 
     * Both cores must always be busy, i.e. while core 1 is collecting and outputting the data core 0 must also be 
     * doing something so it needs to be busy when it is not switching the telemetry collection rate
     */
   
    printf("Offloading collect telemetry to core 1\n");
    multicore_launch_core1(collect_telemetry);          // Start the core 1 thread

    // Start
    stop_telemetry.store(false);
    printf("Putting main core to sleep for 5 seconds\n");
    while (true) { sleep_ms(10000); }

    // // Stop
    // stop_telemetry.store(true);
    // printf("Thread 1 has stopped executing collect_telemetry\n");

    // // Change delay and restart collection
    // set_telemetry_delay(3000);
    // printf("Starting exectution on thread 1 with new delay set\n");
    // stop_telemetry.store(false);
    // sleep_ms(5000);

    // // Stop collection
    // stop_telemetry.store(true);
    // printf("Thread 1 has finished executing stop_telemetry\n");

    return 0;    
}
