#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mpu6050_i2c.h"


int main() {
    stdio_init_all();
    sleep_ms(5000);
    for (int i = 0; i < 10; i++) { printf("TEST TEST TEST\n"); }
    sleep_ms(1000);
    mpu6050_readings();
    return 0;    
}
