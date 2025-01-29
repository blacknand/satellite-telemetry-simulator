/**
 * @file satellite_factory.cpp
 * @brief This file is the Pico SDK specific implementation of the factory functions
 * @paragraph
 * This file is the Pico SDK specific implementation of the factory functions
 * to make an instance of SatelliteInterface that overrides the base SatelliteInterface
 * class so the Qt app (satellite_sim) can use the Pico SDK specific implementation
 * via depdenency injection. Please refer to the UML diagram in the README.md file.
 */


#include "pico_satellite_factory.h"
#include "satellite_telemetry_simulator.h"

#include <memory>


std::unique_ptr<SatelliteInterface> make_satellite_sensors() {
    return std::make_unique<SatelliteSensors>();
}