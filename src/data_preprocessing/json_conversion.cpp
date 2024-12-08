#include "json_conversion.h"

namespace nlohmann { 
    // Specialization for BME280Data
    //void nlohmann::adl_serializer<BME280Data>::to_json(json& j, const BME280Data& d) {
        //j = json{
            //{"environment", {
                //{"temperature (*C)", d.temperature},
                //{"pressure (hPa)", d.pressure},
                //{"altitude (m)", d.altitude},
                //{"humidity (%)", d.humidity}
            //}}
        //};
    //}

    //void nlohmann::adl_serializer<BME280Data>::from_json(const json& j, BME280Data& d) {
        //j.at("temperature").get_to(d.temperature);
        //j.at("pressure").get_to(d.pressure);
        //j.at("altitude").get_to(d.altitude);
        //j.at("humidity").get_to(d.humidity);
    //}

    //// Specialization for MPU6050Data
    //void nlohmann::adl_serializer<MPU6050Data>::to_json(json& j, const MPU6050Data& d) {
        //j = json{
            //{"accelerometer", {
                //{"accel_x", d.accel_x},
                //{"accel_y", d.accel_y},
                //{"accel_z", d.accel_z}
            //}},
            //{"gyroscope", {
                //{"gyro_x", d.gyro_x},
                //{"gyro_y", d.gyro_y},
                //{"gyro_z", d.gyro_z}
            //}}
        //};
    //}

    //void nlohmann::adl_serializer<MPU6050Data>::from_json(const json& j, MPU6050Data& d) {
        //j.at("accel_x").get_to(d.accel_x);
        //j.at("accel_y").get_to(d.accel_y);
        //j.at("accel_z").get_to(d.accel_z);
        //j.at("gyro_x").get_to(d.gyro_x);
        //j.at("gyro_y").get_to(d.gyro_y);
        //j.at("gyro_z").get_to(d.gyro_z);
    //}

    void nlohmann::adl_serializer<SensorData>::to_json(json& j, const SensorData& d) {
        j = json{ 
            {"sensors", 
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
                }}
            }
        };
    }

    void nlohmann::adl_serializer<SensorData>::from_json(const json& j, SensorData& d) {
        const auto& sensors = j.at("sensors");
        const auto& env = sensors.at("environment");
        d.temperature = env.at("temperature (*C)").get<float>();
        d.pressure    = env.at("pressure (hPa)").get<float>();
        d.altitude    = env.at("altitude (m)").get<float>();
        d.humidity    = env.at("humidity (%)").get<float>();

        const auto& accel = sensors.at("accelerometer");
        d.accel_x = accel.at("accel_x").get<float>();
        d.accel_y = accel.at("accel_y").get<float>();
        d.accel_z = accel.at("accel_z").get<float>();

        const auto& gyro = sensors.at("gyroscope");
        d.gyro_x = gyro.at("gyro_x").get<float>();
        d.gyro_y = gyro.at("gyro_y").get<float>();
        d.gyro_z = gyro.at("gyro_z").get<float>();
    }
}