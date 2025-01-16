#include "../common/satellite_data.h"
#include "../satellite-telemetry-simulator/satellite_telemetry_simulator.h"

SatelliteInterface* createSatelliteImplementation() {
    return new SatelliteSensors();
}
