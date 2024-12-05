#include "MPU6050.h"
#include "bme280.h"

MPU6050 mpu6050;
BME280 bme280;

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
    mpu6050.output_data();
    bme280.loop_output();
    delay(5000);
}