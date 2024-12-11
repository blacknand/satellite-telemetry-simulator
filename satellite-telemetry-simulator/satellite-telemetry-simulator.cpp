#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "sensors/MPU6050.h"

MPU6050 mpu;

int main() {
    mpu.init();
    return 0;    
}
