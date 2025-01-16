#ifndef SATELLITE_DATA_H
#define SATELLITE_DATA_H

#include <nlohmann/json.hpp>
#include <cstdint>

using json = nlohmann::json;

struct SatelliteData {
    std::string utc_time;

    int32_t temperature;
    int32_t pressure;
    int32_t altitude;
    int32_t humidity;

    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;

    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;

    // Satellite system data
    // TODO: Add satellite system data
    int16_t mpu_temp;
};

class SatelliteInterface {
public:
    virtual ~SatelliteInterface() = default;    // Declare a virtual destructor
    virtual SatelliteData get_satellite_data() = 0;     // Declare a pure virtual function
};

#endif // SATELLITE_DATA_H