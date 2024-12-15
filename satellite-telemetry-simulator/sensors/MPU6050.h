#ifndef PICO_MPU6050_ACCEL
#define PICO_MPU6050_ACCEL

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>

// https://towardsdev.com/interfacing-the-mpu6050-with-raspberry-pi-pico-w-in-c-176deea5c3af

// I2C defines
#define I2C_PORT i2c0
#define MPU6050_ADDR 0x68

// MPU6050 register addresses
#define REG_PWR_MGMT_1 0x6B
#define REG_ACCEL_XOUT_H 0x3B
#define REG_GYRO_CONFIG 0x1B
#define REG_ACCEL_CONFIG 0x1C
#define REG_SMPLRT_DIV 0x19
#define WHO_AM_I_REG 0x75

// Sensitivity scale factors for different ranges
#define ACCEL_SCALE_FACTOR_2G 16384.0  // for ±2g
#define ACCEL_SCALE_FACTOR_4G 8192.0   // for ±4g
#define ACCEL_SCALE_FACTOR_8G 4096.0   // for ±8g
#define ACCEL_SCALE_FACTOR_16G 2048.0  // for ±16g

#define GYRO_SCALE_FACTOR_250DPS 131.0    // for ±250 degrees per second
#define GYRO_SCALE_FACTOR_500DPS 65.5     // for ±500 degrees per second
#define GYRO_SCALE_FACTOR_1000DPS 32.8    // for ±1000 degrees per second
#define GYRO_SCALE_FACTOR_2000DPS 16.4    // for ±2000 degrees per second

// Select the desired scale factor
#define ACCEL_SCALE_FACTOR ACCEL_SCALE_FACTOR_4G  // Change this to the desired accelerometer range
#define GYRO_SCALE_FACTOR GYRO_SCALE_FACTOR_250DPS // Change this to the desired gyroscope range

// Corresponding configuration values
#define ACCEL_CONFIG_VALUE 0x08  // for ±4g
#define GYRO_CONFIG_VALUE 0x00  // for ±250 degrees per second
#define SAMPLE_RATE_DIV 1  // Sample rate = 1kHz / (1 + 1) = 500Hzi

class MPU6050 {
public:
    void reset();
    void configure();
    void read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp);
    void init();
};

#endif      // PICO_MPU6050_ACCEL