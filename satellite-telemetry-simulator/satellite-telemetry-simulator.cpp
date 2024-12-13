#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "sensors/MPU6050.h"

MPU6050 mpu;

int main() {
    stdio_init_all();
    sleep_ms(5000);
    for (int i = 0; i < 10; i++) { printf("TEST TEST TEST\n"); }
    sleep_ms(1000);
    mpu.init();
    // printf("Hello from PICO\n");
    // while (true) {
    //     printf("TEST TEST TEST\n");
    //     sleep_ms(100);
    // }
    return 0;    
}
