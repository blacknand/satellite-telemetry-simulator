#ifndef JSON_CONVERSION_H
#define JSON_CONVERSION_H

#include <chrono>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct SatelliteData {
    std::string utc_time;

    float temperature;
    float pressure;
    float altitude;
    float humidity;

    float accel_x;
    float accel_y;
    float accel_z;

    float gyro_x;
    float gyro_y;
    float gyro_z;

    // Satellite system data
    // TODO: Add satellite system data
    float mpu_temp;
};

namespace nlohmann {
    template<>
    struct adl_serializer<SatelliteData> {
        static void to_json(json& j, const SatelliteData& d);
        static void from_json(const json& j, SatelliteData& d);
    };
}

#endif // JSON_CONVERSION_H
