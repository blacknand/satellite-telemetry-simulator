#include "../common/satellite_data.h"
#include "../data_preprocessing/json_conversion.h"

#include <gmock/gmock.h>
#include <gmock/gtest.h>


class MockSatSensors : public SatelliteInterface {
    MOCK_METHOD(SatelliteData, get_satellite_data, (), (override));
};