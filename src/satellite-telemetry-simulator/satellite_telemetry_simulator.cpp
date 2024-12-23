#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mpu6050_i2c.h"
#include "bme280_i2c.h"

int main() {
    stdio_init_all();
    sleep_ms(5000);
    int wait_t = 5000;
    while (wait_t != 0) { wait_t--; }   // Allow time to open serial monitor
    sleep_ms(1000);
    mpu6050_readings();
    sleep_ms(1000);
    bme280_readings();
    return 0;    
}
