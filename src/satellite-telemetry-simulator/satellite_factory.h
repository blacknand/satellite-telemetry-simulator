#include "satellite_telemetry_simulator.h"
#include "../common/satellite_data.h"


class SatelliteSensors : public SatelliteInterface {
public:
    SatelliteData get_satellite_data() override;
};

std::unique_ptr<SatelliteInterface> make_satellite_interface();

