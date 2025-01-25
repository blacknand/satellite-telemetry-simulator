#include "../common/satellite_data.h"
#include "../data_preprocessing/json_conversion.h"


class MockSatSensors : public SatelliteInterface {
    SatelliteData get_satellite_data() overide;
};