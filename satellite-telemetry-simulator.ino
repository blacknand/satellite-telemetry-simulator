#include "src/sensors/MPU6050.h"
#include "src/sensors/bme280.h"

#include "src/data_preprocessing/json_conversion.h"

MPU6050 mpu6050;
BME280 bme280;
SensorData sensor_data;

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
    // TODO: UTC in JSON
    auto bme_data = bme280.get_data();
    auto mpu_data = mpu6050.get_data();
    sensor_data.temperature = bme_data.temperature;
    sensor_data.pressure = bme_data.pressure;
    sensor_data.altitude = bme_data.altitude;
    sensor_data.humidity = bme_data.humidity;
    sensor_data.accel_x = mpu_data.accel_x;
    sensor_data.accel_y = mpu_data.accel_y;
    sensor_data.accel_z = mpu_data.accel_z;
    sensor_data.gyro_x = mpu_data.gyro_x;
    sensor_data.gyro_y = mpu_data.gyro_y;
    sensor_data.gyro_z = mpu_data.gyro_z;
    json j = sensor_data;
    Serial.println(j.dump().c_str());
    delay(1000);
}