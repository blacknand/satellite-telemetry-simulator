#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#include "bme280_i2c.h"

int32_t t_fine;

uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
uint8_t dig_H1, dig_H3;
int8_t dig_H6;
int16_t dig_H2, dig_H4, dig_H5;

/* The following compensation functions are required to convert from the raw ADC
data from the chip to something usable. Each chip has a different set of
compensation parameters stored on the chip at point of manufacture, which are
read from the chip at startup and used in these routines.
*/
int32_t compensate_temp(int32_t adc_T) {
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1))) * ((int32_t) dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t) dig_T1)) * ((adc_T >> 4) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3))
            >> 14;

    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

uint32_t compensate_pressure(int32_t adc_P) {
    int32_t var1, var2;
    uint32_t p;
    var1 = (((int32_t) t_fine) >> 1) - (int32_t) 64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t) dig_P6);
    var2 = var2 + ((var1 * ((int32_t) dig_P5)) << 1);
    var2 = (var2 >> 2) + (((int32_t) dig_P4) << 16);
    var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t) dig_P2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t) dig_P1)) >> 15);
    if (var1 == 0)
        return 0;

    p = (((uint32_t) (((int32_t) 1048576) - adc_P) - (var2 >> 12))) * 3125;
    if (p < 0x80000000)
        p = (p << 1) / ((uint32_t) var1);
    else
        p = (p / (uint32_t) var1) * 2;

    var1 = (((int32_t) dig_P9) * ((int32_t) (((p >> 3) * (p >> 3)) >> 13))) >> 12;
    var2 = (((int32_t) (p >> 2)) * ((int32_t) dig_P8)) >> 13;
    p = (uint32_t) ((int32_t) p + ((var1 + var2 + dig_P7) >> 4));

    return p;
}

uint32_t compensate_humidity(int32_t adc_H) {
    int32_t v_x1_u32r;
    v_x1_u32r = (t_fine - ((int32_t) 76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t) dig_H4) << 20) - (((int32_t) dig_H5) * v_x1_u32r)) +
                   ((int32_t) 16384)) >> 15) * (((((((v_x1_u32r * ((int32_t) dig_H6)) >> 10) * (((v_x1_u32r *
                                                                                                  ((int32_t) dig_H3))
            >> 11) + ((int32_t) 32768))) >> 10) + ((int32_t) 2097152)) *
                                                 ((int32_t) dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t) dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

    return (uint32_t) (v_x1_u32r >> 12);
}


void read_compensation_parameters() {
    uint8_t buffer[26];

    // Read temperature and pressure calibration data
    uint8_t reg = 0x88;
    i2c_write_blocking(i2c_default, BME_280_ADDR, &reg, 1, true); // Set register to 0x88
    i2c_read_blocking(i2c_default, BME_280_ADDR, buffer, 24, false); // Read 24 bytes

    // Temperature compensation
    dig_T1 = buffer[0] | (buffer[1] << 8);
    dig_T2 = buffer[2] | (buffer[3] << 8);
    dig_T3 = buffer[4] | (buffer[5] << 8);

    // Pressure compensation
    dig_P1 = buffer[6] | (buffer[7] << 8);
    dig_P2 = buffer[8] | (buffer[9] << 8);
    dig_P3 = buffer[10] | (buffer[11] << 8);
    dig_P4 = buffer[12] | (buffer[13] << 8);
    dig_P5 = buffer[14] | (buffer[15] << 8);
    dig_P6 = buffer[16] | (buffer[17] << 8);
    dig_P7 = buffer[18] | (buffer[19] << 8);
    dig_P8 = buffer[20] | (buffer[21] << 8);
    dig_P9 = buffer[22] | (buffer[23] << 8);

    // Read humidity calibration data
    reg = 0xA1;
    i2c_write_blocking(i2c_default, BME_280_ADDR, &reg, 1, true); // Set register to 0xA1
    i2c_read_blocking(i2c_default, BME_280_ADDR, &dig_H1, 1, false); // Read 1 byte

    reg = 0xE1;
    i2c_write_blocking(i2c_default, BME_280_ADDR, &reg, 1, true); // Set register to 0xE1
    i2c_read_blocking(i2c_default, BME_280_ADDR, buffer, 7, false); // Read 7 bytes

    // Humidity compensation
    dig_H2 = buffer[0] | (buffer[1] << 8);
    dig_H3 = buffer[2];
    dig_H4 = (buffer[3] << 4) | (buffer[4] & 0x0F);
    dig_H5 = (buffer[5] << 4) | (buffer[4] >> 4);
    dig_H6 = buffer[6];
}


static void bme280_read_raw(int32_t *humidity, int32_t *pressure, int32_t *temperature) {
    uint8_t buffer[8];
    uint8_t reg = 0xF7;

    // Set register to 0xF7
    i2c_write_blocking(i2c_default, BME_280_ADDR, &reg, 1, true);
    // Read 8 bytes of raw data
    i2c_read_blocking(i2c_default, BME_280_ADDR, buffer, 8, false);

    // Extract raw data
    *pressure = ((uint32_t)buffer[0] << 12) | ((uint32_t)buffer[1] << 4) | (buffer[2] >> 4);
    *temperature = ((uint32_t)buffer[3] << 12) | ((uint32_t)buffer[4] << 4) | (buffer[5] >> 4);
    *humidity = ((uint32_t)buffer[6] << 8) | buffer[7];
}


void bme280_readings() {
#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #warning bme280_i2c.c requires a board with I2C pins
    puts("Default I2C pins were not defined");
    return;
#else
    printf("BME280 reading data from registers...\n");

    // Initialize I2C
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    // Read compensation parameters
    read_compensation_parameters();

    // Configure humidity oversampling
    uint8_t config[2];
    config[0] = 0xF2; // Register address for humidity oversampling
    config[1] = 0x01; // Oversampling x1
    i2c_write_blocking(i2c_default, BME_280_ADDR, config, 2, false);

    // Configure measurement control
    config[0] = 0xF4; // Register address for measurement control
    config[1] = 0x27; // Normal mode, temperature/pressure oversampling x1
    i2c_write_blocking(i2c_default, BME_280_ADDR, config, 2, false);

    int32_t humidity, pressure, temperature;

    // Infinite loop to read and display sensor data
    while (1) {
        bme280_read_raw(&humidity, &pressure, &temperature);

        // Convert raw data to human-readable format
        temperature = compensate_temp(temperature);
        pressure = compensate_pressure(pressure);
        humidity = compensate_humidity(humidity);

        printf("Humidity = %.2f%%\n", humidity / 1024.0);
        printf("Pressure = %dPa\n", pressure);
        printf("Temp. = %.2fC\n", temperature / 100.0);

        sleep_ms(1000); // Delay for readability
    }
#endif
}
