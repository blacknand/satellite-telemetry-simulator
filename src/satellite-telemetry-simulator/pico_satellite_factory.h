#ifndef PICO_SATELLITE_FACTORY_H
#define PICO_SATELLITE_FACTORY_H

#include "../common/satellite_data.h"


class SatelliteSensors : public SatelliteInterface {
public:
    SatelliteData get_satellite_data() override;
};

std::unique_ptr<SatelliteInterface> make_satellite_sensors();

#endif // PICO_SATELLITE_FACTORY_H