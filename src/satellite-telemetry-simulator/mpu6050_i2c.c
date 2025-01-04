#include "mpu6050_i2c.h"

// Define global variables to store offsets
int16_t accel_offsets[3] = {0, 0, 0};
int16_t gyro_offsets[3] = {0, 0, 0};
float filtered_accel[3] = {0.0, 0.0, 0.0};
float filtered_gyro[3] = {0.0, 0.0, 0.0};
#define ALPHA 0.475

static void mpu6050_reset() {
    uint8_t buf[] = {0x6B, 0x80};
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
    sleep_ms(100); // Allow device to reset and stabilize

    // Clear sleep mode (0x6B register, 0x00 value)
    buf[1] = 0x00;  // Clear sleep mode by writing 0x00 to the 0x6B register; PWR_MGMT_1
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false); 
    sleep_ms(100);   // Allow stabilization after waking up
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
        int16_t raw_accel = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]) - accel_offsets[i];
        filtered_accel[i] = ALPHA * raw_accel + (1 - ALPHA) * filtered_accel[i];
        accel[i] = (int16_t)filtered_accel[i]; // Cast back to int16_t if needed
    }

    // Now gyro data from reg 0x43 for 6 bytes
    val = 0x43;
    i2c_write_blocking(i2c_default, ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        int16_t raw_gyro = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]) - gyro_offsets[i];
        filtered_gyro[i] = ALPHA * raw_gyro + (1 - ALPHA) * filtered_gyro[i];
        gyro[i] = (int16_t)filtered_gyro[i]; // Cast back to int16_t if needed
    }

    // Now temperature from reg 0x41 for 2 bytes
    val = 0x41;
    i2c_write_blocking(i2c_default, ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, ADDR, buffer, 2, false); 

    *temp = buffer[0] << 8 | buffer[1];
}


void set_offsets(int16_t accel[3], int16_t gyro[3]) {
    for (int i = 0; i < 3; i++) {
        accel_offsets[i] = accel[i];
        gyro_offsets[i] = gyro[i];
    }
    accel_offsets[2] -= 16384;       // Adjust Z-Axis for accelerometer to account for 1g (16384 LSB/g at ±2g range)
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
     * so periodic recalibration is recommended.
     */
    // [0] - X-Axis
    // [1] - Y-Axis
    // [2] - Z-Axis
    int16_t acceleration[3], gyro[3], temp;
    int32_t total_acceleration[3] = {0, 0, 0};
    int32_t total_gyro[3] = {0, 0, 0};
    int32_t total_temp = 0;
    int16_t avg_acceleration[3], avg_gyro[3];
    int samples = 5000;

    for (int i = 0; i < samples; i++) {
        mpu6050_read_raw(acceleration, gyro, &temp);
        for (int j = 0; j < 3; j++) {
            total_acceleration[j] += acceleration[j];
            total_gyro[j] += gyro[j];
        }
        total_temp += temp;
        sleep_ms(10);
    }

    // Calculate averages
    for (int i = 0; i < 3; i++) {
        avg_acceleration[i] = total_acceleration[i] / samples;
        avg_gyro[i] = total_gyro[i] / samples;
        printf("avg_accel[%d]: %d\n", i, avg_acceleration[i]);
        printf("avg_gyro[%d]: %d\n", i, avg_gyro[i]);
    }
    int32_t avg_temp = total_temp / samples;
    printf("avg_temp: %d\n", avg_temp);

    // Calculate offset values
    set_offsets(avg_acceleration, avg_gyro);

    printf("Accel Offsets: X=%d, Y=%d, Z=%d\n", accel_offsets[0], accel_offsets[1], accel_offsets[2]);
    printf("Gyro Offsets: X=%d, Y=%d, Z=%d\n", gyro_offsets[0], gyro_offsets[1], gyro_offsets[2]);
}


static bool mpu6050_test() {
    // Check sensor is not in sleep mode
    uint8_t reg_addr = 0x6B;
    uint8_t reg_val = 0;
    i2c_write_blocking(i2c_default, ADDR, &reg_addr, 1, true);
    i2c_read_blocking(i2c_default, ADDR, &reg_val, 1, false);
    bool is_asleep = (reg_val & (1 << 6)) != 0;
    if (is_asleep) {
        printf("MPU6050 sensor is alseep\n");
        return false;
    }

    // Configure accelerometer and gyroscope range
    uint8_t buf[2];
    buf[0] = 0x1C;
    buf[1] = (1 << 7) | (1 << 6) | (1 << 5);  // XA_ST, YA_ST, ZA_ST
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    buf[0] = 0x1B;
    buf[1] = (1 << 7) | (1 << 6) | (1 << 5);  // XG_ST, YG_ST, ZG_ST
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);

    sleep_ms(100);

    // Get factory trim values for gyroscope
    uint8_t self_test_reg[3] = {0x0D, 0x0E, 0x0F};  
    uint8_t g_test_vars[3] = {0, 0, 0};
    uint8_t a_test_vars[3] = {0, 0, 0};
    reg_val = 0;
    for (int i = 0; i < 3; i++) {
        i2c_write_blocking(i2c_default, ADDR, &self_test_reg[i], 1, true);
        i2c_read_blocking(i2c_default, ADDR, &reg_val, 1, false);
        g_test_vars[i] = reg_val & 0x1F;
        a_test_vars[i] = (reg_val & 0xE0) >> 5;
    }

    // Calculate factory trim values, full scale range should be set to +250dps (already done by default)
    // Equations on page 10: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
    float ft_accel[3] = {0, 0, 0};
    float ft_gyro[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
        if (g_test_vars[i] != 0) {
            if (i == 0) {  // X-axis (FT[Xg])
                ft_gyro[i] = 25.0 * 131.0 * pow(1.046, g_test_vars[i] - 1.0);
            } else if (i == 1) {  // Y-axis (FT[Yg])
                ft_gyro[i] = -25.0 * 131.0 * pow(1.046, g_test_vars[i] - 1.0);
            } else if (i == 2) {  // Z-axis (FT[Zg])
                ft_gyro[i] = 25.0 * 131.0 * pow(1.046, g_test_vars[i] - 1.0);
            }
        } else 
            ft_gyro[i] = 0.0;
    }

    // Full-scale range should be set to +8g (already done by default)
    for (int i = 0; i < 3; i++) {
        for (int i = 0; i < 3; i++) {
            if (a_test_vars[i] != 0) {
                ft_accel[i] = 4096.0 * 0.34 * (pow(0.92, ((a_test_vars[i] - 1.0) / pow(2, 3))) / 0.34);
            } else {
                ft_accel[i] = 0.0;  // If TEST = 0, FT = 0
            }
        }
    }

    int16_t self_test_accel[3], self_test_gyro[3], temp;
    int16_t accel[3], gyro[3];

    // Enable self-test
    // mpu6050_config();

    mpu6050_read_raw(self_test_accel, self_test_gyro, &temp);
    mpu6050_reset();
    sleep_ms(100);
    mpu6050_read_raw(accel, gyro, &temp);

    for (int i = 0; i < 3; i++) {
        if (ft_accel[i] == 0.0 || ft_gyro[i] == 0.0) {
            printf("Factory Trim Value is zero for Axis %d. Cannot calculate deviation.\n", i);
            return false;
        }

        // Scale raw values for deviation calculation
        float scaled_self_test_accel = self_test_accel[i] / 16384.0;  // Convert to g
        float scaled_accel = accel[i] / 16384.0;                      // Convert to g
        float scaled_self_test_gyro = self_test_gyro[i] / 131.0;      // Convert to °/s
        float scaled_gyro = gyro[i] / 131.0;                          // Convert to °/s

        float accel_deviation = (scaled_self_test_accel - scaled_accel) / ft_accel[i];
        float gyro_deviation = (scaled_self_test_gyro - scaled_gyro) / ft_gyro[i];

        // printf("Accel Deviation Axis %d: %.2f%%\n", i, accel_deviation * 100);
        // printf("Gyro Deviation Axis %d: %.2f%%\n", i, gyro_deviation * 100);

        if (accel_deviation < -0.14 || accel_deviation > 0.14 ||
            gyro_deviation < -0.14 || gyro_deviation > 0.14) {
            printf("Self-Test Failed on Axis %d\n", i);
            return false;
        }
    }
    return true;
}


void mpu6050_init(){
#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #warning mpu6050_i2c.c requires a board with I2C pins
    puts("Default I2C pins were not defined");
    return 0;
#else
    printf("MPU6050 reading data from registers...\n");

    // I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    mpu6050_reset();
    // mpu6050_calibrate();
    bool test = mpu6050_test();
    if (!test) {
        printf("MPU6050 test failed");
        return;
    }

    mpu6050_calibrate();
    mpu6050_output();   // Output data from func instead of setting up I2C wire and then reading data each time this function is called
#endif
}


void mpu6050_output() {
    int16_t acceleration[3], gyro[3], temp;
    mpu6050_read_raw(acceleration, gyro, &temp);
    printf("Acc. X = %d, Y = %d, Z = %d\n", acceleration[0], acceleration[1], acceleration[2]);
    printf("Gyro. X = %d, Y = %d, Z = %d\n", gyro[0], gyro[1], gyro[2]);
    printf("Temp. = %f\n", (temp / 340.0) + 36.53); 
}


struct MPUDataStruct mpu6050_get_data() {
    int16_t acceleration[3], gyro[3], temp;
    mpu6050_read_raw(acceleration, gyro, &temp);
    struct MPUDataStruct return_data;
    for (int i = 0; i < 3; i++) {
        return_data.acceleration[i] = acceleration[i];
        return_data.gyro[i] = gyro[i];
    }
    return_data.temp = temp;
    return return_data;
}