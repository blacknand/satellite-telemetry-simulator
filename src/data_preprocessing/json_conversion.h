#ifndef JSON_CONVERSION_H
#define JSON_CONVERSION_H

#include "/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/json.hpp"
#include "/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/sensors/bme280.h"
#include "/Users/nathanblackburn/programming/satellite-telemetry-simulator/src/sensors/MPU6050.h"

using json = nlohmann::json;

// NLOHMANN_JSON_NAMESPACE_BEGIN

// Specialization for BME280Data
namespace nlohmann {
    template <>
    struct adl_serializer<BME280Data> {
        static void to_json(json& j, const BME280Data& data);
        static void from_json(const json& j, BME280Data& data);
    };

    // Specialization for MPU6050Data
    template <>
    struct adl_serializer<MPU6050Data> {
        static void to_json(json& j, const MPU6050Data& data);
        static void from_json(const json& j, MPU6050Data& data);
    };

// NLOHMANN_JSON_NAMESPACE_END
}

#endif // JSON_CONVERSION_H
