#ifndef MPU6050_I2C_H
#define MPU6050_I2C_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ADDR 0x68

#ifdef i2c_default
static void mpu6050_reset();
static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t* temp);
static void mpu6050_config();
static void mpu6050_calibrate();
static bool mpu6050_test();
void mpu6050_init();
void mpu6050_output();
void set_offsets(int16_t avg_accel[3], int16_t avg_gyro[3]);
#endif  // i2c_default

#ifdef __cplusplus
}
#endif

#endif  // MPU6050_I2C_H