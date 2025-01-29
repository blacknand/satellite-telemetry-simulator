#include "q_satellite_factory.h"

class MockSatelliteSensors : public SatelliteInterface {
public:
    SatelliteData get_satellite_data() override {
        SatelliteData data;
        data.temperature = 25;
        data.pressure = 1013;
        data.humidity = 50;
        return data;
    }
};

std::unique_ptr<SatelliteInterface> make_satellite_sensors() {
    return std::make_unique<MockSatelliteSensors>();
}
