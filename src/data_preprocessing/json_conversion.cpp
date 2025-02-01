#include "json_conversion.h"
#include <iostream>

namespace nlohmann { 
    void nlohmann::adl_serializer<SatelliteData>::to_json(json& j, const SatelliteData& d) {
        // std::cout << d.gyro_z << std::endl;
        j = json{
            {"utc data", d.utc_time}, 
            {"sensor data", 
                {"accelerometer", {
                    {"accel_x", d.accel_x},
                    {"accel_y", d.accel_y},
                    {"accel_z", d.accel_z}
                }},
                {"gyroscope", {
                    {"gyro_x", d.gyro_x},
                    {"gyro_y", d.gyro_y},
                    {"gyro_z", d.gyro_z}
                }},
                {"environment", {
                    {"temperature (*C)", d.temperature},
                    {"pressure (hPa)", d.pressure},
                    {"altitude (m)", d.altitude},
                    {"humidity (%)", d.humidity}
                }},
            {"sensor meta data", {
                {"mpu temperature", d.mpu_temp}
            }}}
        };
    }

    void nlohmann::adl_serializer<SatelliteData>::from_json(const json& j, SatelliteData& d) {
        const auto& sensors = j.at("sensors");
        const auto& env = sensors.at("environment");
        d.temperature = env.at("temperature (*C)").get<int32_t>();
        d.pressure    = env.at("pressure (hPa)").get<int32_t>();
        d.altitude    = env.at("altitude (m)").get<int32_t>();
        d.humidity    = env.at("humidity (%)").get<int32_t>();

        const auto& accel = sensors.at("accelerometer");
        d.accel_x = accel.at("accel_x").get<int16_t>();
        d.accel_y = accel.at("accel_y").get<int16_t>();
        d.accel_z = accel.at("accel_z").get<int16_t>();

        const auto& gyro = sensors.at("gyroscope");
        d.gyro_x = gyro.at("gyro_x").get<int16_t>();
        d.gyro_y = gyro.at("gyro_y").get<int16_t>();
        d.gyro_z = gyro.at("gyro_z").get<int16_t>();
    }
}