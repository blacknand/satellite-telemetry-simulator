#ifndef SATELLITE_TELEMETRY_SIMULATOR_H
#define SATELLITE_TELEMETRY_SIMULATOR_H

#pragma once

#include <nlohmann/json.hpp>

#include "mpu6050_i2c.h"
#include "bme280_i2c.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../common/utc_data.h"
#include "../common/satellite_data.h"
#include "../data_preprocessing/json_conversion.h"


using json = nlohmann::json;
json j;

class SatelliteSensors : public SatelliteInterface {
    public:
        SatelliteData getSatelliteData() override;
};

BME280Data bme280_data;
MPU6050Data mpu6050_data;
SatelliteData satellite_data;
MPUDataStruct _mpu_data_struct;


#endif  // SATELLITE_TELEMETRY_SIMULATOR_H