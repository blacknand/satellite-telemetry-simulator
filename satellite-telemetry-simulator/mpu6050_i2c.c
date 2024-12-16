#include "mpu6050_i2c.h"


static void mpu6050_reset() {
    uint8_t buf[] = {0x6B, 0x80};
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
    sleep_ms(100); // Allow device to reset and stabilize

    // Clear sleep mode (0x6B register, 0x00 value)
    buf[1] = 0x00;  // Clear sleep mode by writing 0x00 to the 0x6B register; PWR_MGMT_1
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 
    sleep_ms(10);   // Allow stabilization after waking up
}


static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    /* For this particular device, we send the device the register we want to read
     * first, then subsequently read from the device. The register is auto incrementing
     * so we don't need to keep sending the register we want, just the first.
     * 
     * Because we are using I2C, we write the register ADDRess we want to read from
     * to the MPU6050 first and then after this write reading starts from the specified
     * register.
     */

    uint8_t buffer[6];

    // Start reading acceleration registers from register 0x3B for 6 bytes
    uint8_t val = 0x3B;
    i2c_write_blocking(i2c_default, ADDR, &val, 1, true);       // Keep master control of bus
    i2c_read_blocking(i2c_default, ADDR, buffer, 6, false);     // False, finished with bus

    /**
     * Bit shift each axis data by 8 bits left to read from the 
     * high byte and the low byte to reconstruct each 16 bit value
     * from all axises
     */
    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Now gyro data from reg 0x43 for 6 bytes
    val = 0x43;
    i2c_write_blocking(i2c_default, ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);;
    }

    // Now temperature from reg 0x41 for 2 bytes
    val = 0x41;
    i2c_write_blocking(i2c_default, ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buffer, 2, false); 

    *temp = buffer[0] << 8 | buffer[1];
}


static void mpu6050_config() {
    uint8_t buf[2];

    // Self test accelerometer and set range
    uint8_t accel_config = 0x00;
    // Set the X, Y and Z accelerometer config registers to perform self test
    // Self-test response = Sensor output with self-test enabled – Sensor output without self-test enabled
    // https://www.learn-c.org/en/Bitmasks
    accel_config |= (1 << 7);       // XA_ST
    accel_config |= (1 << 6);       // YA_ST
    accel_config |= (1 << 5);       // ZA_ST
    // AF_SEL bits already at 0
    buf[0] = 0x1C;
    buf[1] = accel_config;

    // Set range
    // ± 2g is the most suitable range for this as it is to simulate a satellite in orbit, so it will not experience large accelerations
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);   // ± 2g; AFS_SEL register
    memset(buf, 0, sizeof buf);

    // Self test gyroscope and set range
    uint8_t gyro_config = 0x00;
    // Set X, Y and Z gyroscope config registers to perform self test
    gyro_config |= (1 << 7);         // XG_ST
    gyro_config |= (1 << 6);         // YG_ST
    gyro_config |= (1 << 5);         // ZG_ST
    // FS_SEL already at 0
    buf[0] = 0x1B;
    buf[1] = gyro_config;

    // Set range
    // ±250dp
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}


static void mpu6050_calibrate() {
    /**
     * Calibrate the accelerometer and gyroscope.
     * This works by calculating the average values on all three axis
     * for both the accelerometer and the gyroscope and then calculating
     * the offset values for these averages and writing them to the 
     * registers that store the raw data for the accelerometer and gyroscope.
     * 
     * The temperature can affec the readings
     * so periodic recalibration is recommended
     */
    // [0] - X-Axis
    // [1] - Y-Axis
    // [2] - Z-Axis
    int16_t acceleration[3], gyro[3], temp;
    int32_t* total_acceleration = (int32_t*)calloc(3, sizeof(int32_t));
    int32_t* total_gyro = (int32_t*)calloc(3, sizeof(int32_t));
    int32_t* total_temp = (int32_t*)malloc(sizeof(int32_t)); 
    int16_t avg_acceleration[3], avg_gyro[3];
    int32_t avg_temp;
    int samples = 1500, counter = 1500;
    if (!total_acceleration || !total_gyro || !total_temp)
        return;
    while (counter) {
        mpu6050_read_raw(acceleration, gyro, &temp);
        for (int i = 0; i < 3; i++) {
            total_acceleration[i] += acceleration[i];
            total_gyro[i] += gyro[i];
        }
        *total_temp += temp;
        sleep_ms(10);   // Delay between samples
        counter--;
    }
    // Calculate averages
    for (int i = 0; i < 3; i++) {
        avg_acceleration[i] = total_acceleration[i] / samples;
        avg_gyro[i] = total_gyro[i] / samples;
    }
    avg_temp = *total_temp / samples;
    free(total_acceleration);
    free(total_gyro);
    free(total_temp);

    // Calculate offset values
    int16_t accel_offsets[3], gyro_offsets[3];
    for (int i = 0; i < 3; i++) {
        accel_offsets[i] = -avg_acceleration[i];
        gyro_offsets[i] = -avg_gyro[i];
    }

    // TODO: Write to correct offset registers 

    accel_offsets[2] -= 16384;       // Adjust Z-Axis for accelerometer to account for 1g (16384 LSB/g at ±2g range)
    uint8_t buffer[2];
    for (int i = 0; i < 3; i++) {
        // Shift the high byte to low byte position and combine with 0xFF to get the high byte
        buffer[0] = (accel_offsets[i] >> 8) &  0xFF;
        buffer[1] = accel_offsets[i] & 0xFF;
        i2c_write_blocking(i2c_default, ADDR, buffer, 0x3B, false);
    }

    for (int i = 0; i < 3; i++) {
        buffer[0] = (gyro_offsets[i] >> 8) & 0xFF;
        buffer[1] = gyro_offsets[i] & 0xFF;
        i2c_write_blocking(i2c_default, ADDR, buffer, 0x3B, false);
    } 
}


static bool mpu6050_test() {
    // Self-test response = Sensor output with self-test enabled – Sensor output without self-test enabled
    // Self test response minimum and maximum accepted values are -14 and +14
    int16_t self_test_accel[3], self_test_gyro[3], temp;
    mpu6050_read_raw(self_test_accel, self_test_gyro, &temp);
    mpu6050_reset();    // Reset to get raw values without performing self test
    int16_t accel[3], gyro[3];
    mpu6050_read_raw(accel, gyro, &temp);   // Do not need to create new temp since it is not tested

    int16_t accel_test_result[3], gyro_test_result[3];
    for (int i = 0; i < 3; i++) {
        accel_test_result[i] = self_test_accel[i] - accel[i];
        gyro_test_result[i] = self_test_gyro[i] - gyro[i];
        if (accel_test_result[i] < -14 || accel_test_result[i] > 14 ||
            gyro_test_result[i] < -14 || gyro_test_result[i] > 14)
            return false;
    }
    return true;
}


void mpu6050_readings(){
#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #warning mpu6050_i2c.c requires a board with I2C pins
    puts("Default I2C pins were not defined");
    return 0;
#else
    printf("MPU6050 reading raw data from registers...\n");

    // I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    int16_t acceleration[3], gyro[3], temp;

    /**
     * Order of functions:
     * 1. Call config first to configure sensor
     * 2. Calibrate sensor for accurate readings
     * 3. Test sensor which requires reseting sensor for raw readings
     * 4. If test passed then call config and calibrate again
     */
    mpu6050_reset();
    mpu6050_config();
    mpu6050_calibrate();
    bool test = mpu6050_test();
    if (!test)
        return;
    mpu6050_config();
    mpu6050_calibrate();

    while (1) {
        mpu6050_read_raw(acceleration, gyro, &temp);

        printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
        printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);
        // TODO: get deg C from register sheet.
        printf("Temp. = %f\n", (temp / 340.0) + 36.53);

        sleep_ms(100);
    }
#endif
}
