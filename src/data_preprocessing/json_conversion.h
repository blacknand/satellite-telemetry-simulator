#ifndef JSON_CONVERSION_H
#define JSON_CONVERSION_H

#include "/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/json.hpp"
#include "/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/sensors/bme280.h"
#include "/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/sensors/MPU6050.h"

using json = nlohmann::json;

struct SensorData {
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
};

// NLOHMANN_JSON_NAMESPACE_BEGIN

// Specialization for BME280Data
namespace nlohmann {
    // template <>
    // struct adl_serializer<BME280Data> {
    //     static void to_json(json& j, const BME280Data& data);
    //     static void from_json(const json& j, BME280Data& data);
    // };

    // // Specialization for MPU6050Data
    // template <>
    // struct adl_serializer<MPU6050Data> {
    //     static void to_json(json& j, const MPU6050Data& data);
    //     static void from_json(const json& j, MPU6050Data& data);
    // };

    template<>
    struct adl_serializer<SensorData> {
        static void to_json(json& j, const SensorData& d);
        static void from_json(const json& j, SensorData& d);
    };

// NLOHMANN_JSON_NAMESPACE_END
}

#endif // JSON_CONVERSION_H
