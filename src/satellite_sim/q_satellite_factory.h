#ifndef Q_SATELLITE_FACTORY_H
#define Q_SATELLITE_FACTORY_H

#include "../common/satellite_data.h"
// #include "../satellite-telemetry-simulator/pico_satellite_factory.h"
#include <memory>


class SatelliteInterface;

// Dependency injection, SatFactory will interfact with satellite data abstractly
// without relying on the Pico SDK specific headers in the Qt app
class SatFactory {
public: 
    SatFactory(std::unique_ptr<SatelliteInterface>&& sensors) 
                : satData(std::move(sensors)) {}
    SatelliteData get_satellite_data();
private:
    std::unique_ptr<SatelliteInterface> satData;
};

std::unique_ptr<SatelliteInterface> make_satellite_sensors();

#endif // Q_SATELLITE_FACTORY_H