#ifndef SATELLITE_FACTORY_H
#define SATELLITE_FACTORY_H

#include "satellite_factory.h"
#include "../common/satellite_data.h"

class MockSatelliteSensors : public SatelliteInterface {
public:
    SatelliteData get_satellite_data() override; 
};

SatelliteInterface* createSatelliteImplementation();

#endif // SATELLITE_FACTORY_H