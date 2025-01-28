#include "satellite_factory.h"

SatelliteData MockSatelliteSensors::get_satellite_data() {
    SatelliteData data;
    data.utc_time = "2021-09-01T12:00:00Z";
    data.temperature = 25;
    data.pressure = 1013;
    data.altitude = 0;
    data.humidity = 50;
    data.accel_x = 0;
    data.accel_y = 0;
    data.accel_z = 0;
    data.gyro_x = 0;
    data.gyro_y = 0;
    data.gyro_z = 0;
    data.mpu_temp = 25;
    return data;
}

SatelliteInterface* createSatelliteImplementation() {
    return new MockSatelliteSensors();
}   