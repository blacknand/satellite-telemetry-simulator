#include <utc_data.h>

#include "src/sensors/MPU6050.h"
#include "src/sensors/bme280.h"
#include "src/data_preprocessing/json_conversion.h"

MPU6050 mpu6050;
BME280 bme280;
SatelliteData satellite_data;

void setup(void) {
  Serial.begin(115200);
  delay(2000);
  while (!Serial)
    delay(10); 

  mpu6050.init();
  Serial.println();
  bme280.init();

  Serial.println("");
  delay(100);
}


void loop() {
    std::string utc_time = get_utc_time();
    auto bme_data = bme280.get_data();
    auto mpu_data = mpu6050.get_data();
    satellite_data.utc_time = utc_time;
    satellite_data.temperature = bme_data.temperature;
    satellite_data.pressure = bme_data.pressure;
    satellite_data.altitude = bme_data.altitude;
    satellite_data.humidity = bme_data.humidity;
    satellite_data.accel_x = mpu_data.accel_x;
    satellite_data.accel_y = mpu_data.accel_y;
    satellite_data.accel_z = mpu_data.accel_z;
    satellite_data.gyro_x = mpu_data.gyro_x;
    satellite_data.gyro_y = mpu_data.gyro_y;
    satellite_data.gyro_z = mpu_data.gyro_z;
    json j = satellite_data;
    Serial.println(j.dump().c_str());
    delay(1000);
}