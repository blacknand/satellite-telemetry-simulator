#ifndef JSON_CONVERSION_H
#define JSON_CONVERSION_H

#include <chrono>

#include <nlohmann/json.hpp>

#include "../common/satellite_data.h"

using json = nlohmann::json;

namespace nlohmann {
    template<>
    struct adl_serializer<SatelliteData> {
        static void to_json(json& j, const SatelliteData& d);
        static void from_json(const json& j, SatelliteData& d);
    };
}

#endif // JSON_CONVERSION_H
