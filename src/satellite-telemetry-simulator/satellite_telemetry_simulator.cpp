#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mpu6050_i2c.h"
#include "bme280_i2c.h"

int main() {

    stdio_init_all();
    sleep_ms(100);      // Allow device to stabalise

    mpu6050_init();
    bme280_init();

    for (int i = 0; i < 10; i++) {
        mpu6050_output();
        bme280_output();
        sleep_ms(1000);
    }
    return 0;    
}
