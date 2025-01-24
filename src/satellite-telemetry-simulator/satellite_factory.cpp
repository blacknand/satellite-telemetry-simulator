#include "satellite_factory.h"

#include <memory>


std::unique_ptr<SatelliteInterface> make_satellite_interface() {
    return std::make_unique<SatelliteSensors>();
}

