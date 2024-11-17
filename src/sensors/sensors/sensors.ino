/*
  > Integration of both the BME280 and MPU6050 sensors to output in a single sketch
  > relays the raw data from both sensors
  > author: nblackburndeveloper@icloud.com; github@blacknand
*/

#include <vector>
#include <Wire.h>
#include <SPI.h>

#include "bme280.h"

BME280 bme280;

/*
  > Using I2C communication protocol on both the BME280 and MPU6050 sensors
  > this prevents having to modify both sensors at the firmware level to make them use the same baud rate
  > meaning the baud rate used here for the serial output is purely for displaying output in the terminal
*/

// TODO: Now integrate both MPU and BME to work in same sketch

void setup() {
  Serial.begin(115200);
  while (!Serial);                            // Wait for serial to initialise before continuing

  bme280.init();
}

void loop() {
  bme280.output_data();
}
