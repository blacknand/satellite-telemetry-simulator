#include <stdio.h>
#include <atomic>
#include <chrono>
// #include <thread>

#include "pico/stdlib.h"
#include "pico/multicore.h"
// #include "pico/mutex.h"         // ?

#include "satellite_telemetry_simulator.h"
#include "set_telemetry.h"
#include "../common/satellite_data.h"
#include "kalman_filter.h"


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
    satellite_data.gyro_x = _mpu_data_struct.gyro[0];
    satellite_data.gyro_y = _mpu_data_struct.gyro[1];
    satellite_data.gyro_z = _mpu_data_struct.gyro[2];

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
        if (!stop_telemetry.load()) {
            j = get_satellite_data();
            SatelliteData local_data;
            local_data.accel_x = _mpu_data_struct.acceleration[0];
            local_data.accel_y = _mpu_data_struct.acceleration[1];
            local_data.accel_z = _mpu_data_struct.acceleration[2];
            float accel[3] = {local_data.accel_x, local_data.accel_y, local_data.accel_z};
            kalman_predict(&kf_state, &kf, accel);
            float measured_v[3] = {0, 0, 0}; // Assume stationary for now
            kalman_update(&kf_state, &kf, measured_v);
            printf("START OF JSON OBJECT\n");
            printf("%s\n", j.dump(4).c_str());
            printf("Kalman: px=%.2f, py=%.2f, pz=%.2f, vx=%.2f, vy=%.2f, vz=%.2f\n",
                   kf_state.state[0], kf_state.state[1], kf_state.state[2],
                   kf_state.state[3], kf_state.state[4], kf_state.state[5]);
            fflush(stdout);
            printf("END OF JSON OBJECT\n");
            sleep_ms(telemetry_delay.load());
        } else {
            sleep_ms(100);
        }
    }
}


void set_telemetry_delay(int new_delay) {
    stop_telemetry.store(true);             // Stop
    telemetry_delay.store(new_delay);
    stop_telemetry.store(false);            // Start
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

    init_kalman(&kf_state, &kf, telemetry_delay.load() / 1000.0f);
    kf_initialised = true;

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

    // NOTE: When WIFI is implemented this method of polling will not work
    // probably will have to do it with sockets or something similar (thinking ZeroMQ or some Boost library)
    char line[256];
    int pos = 0;  // Core 0
    while (true) {
        int c = getchar_timeout_us(100000);
        if (c != PICO_ERROR_TIMEOUT) {
            if (c == '\n' || c == '\r') {
                if (pos > 0) {  // Only process non-empty lines
                    line[pos] = '\0';
                    std::string line_str(line);
                    if (!line_str.empty() && line_str.back() == '\r') {
                        line_str.pop_back();
                    }
                    printf("Received: [%s]\n", line_str.c_str());
                    fflush(stdout);  // Ensure the output is flushed immediately
    
                    if (line_str == "[COMM] rate_change_0.5x")
                        set_telemetry_delay(500);
                    else if (line_str == "[COMM] rate_change_1x")
                        set_telemetry_delay(1000);
                    else if (line_str == "[COMM] rate_change_1.25x")
                        set_telemetry_delay(1250);
                    else if (line_str == "[COMM] rate_change_1.5x")
                        set_telemetry_delay(1500);
                    else if (line_str == "[COMM] rate_change_2x")
                        set_telemetry_delay(2000);
                    else if (line_str == "[COMM] rate_change_2.5x")
                        set_telemetry_delay(2500);
                    else if (line_str == "[COMM] rate_change_3x")
                        set_telemetry_delay(3000);
                    else if (line_str == "[COMM] rate_change_10x")
                        set_telemetry_delay(10000);
                }
                pos = 0;
            } else if (pos < (int)sizeof(line) - 1) {
                line[pos++] = c;
            }
        }
    }
    return 0;    
}
