#ifdef __cplusplus
extern "C" {
#endif

#ifndef BME_280_I2C_H
#define BME_280_I2C_H

#define BME_280_ADDR 0x77
#define READ_BIT 0x80

int32_t compensate_temp(int32_t adc_T);
uint32_t compensate_pressure(int32_t adc_P);
uint32_t compensate_humidity(int32_t adc_H);
void read_compensation_parameters();
static void bme280_read_raw(int32_t *humidity, int32_t *pressure, int32_t *temperature);
void bme280_readings();
#endif  // BME_280_I2C_H

#ifdef __cplusplus
}
#endif