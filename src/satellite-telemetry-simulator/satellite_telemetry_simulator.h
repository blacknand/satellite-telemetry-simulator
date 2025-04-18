#ifndef SATELLITE_TELEMETRY_SIMULATOR_H
#define SATELLITE_TELEMETRY_SIMULATOR_H

#pragma once

#include <nlohmann/json.hpp>
#include <cstdint>
#include <memory>

#include "mpu6050_i2c.h"
#include "bme280_i2c.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../common/utc_data.h"
#include "../common/satellite_data.h"
#include "../data_preprocessing/json_conversion.h"
#include "kalman_filter.h"


using json = nlohmann::json;

json get_satellite_data();
// void collect_telemetry(int delay);
// void start_telemetry(int delay);

extern BME280Data bme280_data;
extern MPU6050Data mpu6050_data;
extern SatelliteData satellite_data;
extern MPUDataStruct _mpu_data_struct;
extern json j;
extern bool calibrated;
extern KalmanState kf_state;    
extern KalmanFilter kf;         
extern bool kf_initialized;    

#endif  // SATELLITE_TELEMETRY_SIMULATOR_H