#ifndef SATELLITE_DATA_H
#define SATELLITE_DATA_H

#include <nlohmann/json.hpp>
#include <cstdint>
#include <memory>


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


// // Abstract base class
// class SatelliteInterface {
// public:
    // virtual SatelliteData get_satellite_data() = 0;
    // virtual ~SatelliteInterface() = default;
// };

// std::unique_ptr<SatelliteInterface> make_satellite_interface();

#endif // SATELLITE_DATA_H