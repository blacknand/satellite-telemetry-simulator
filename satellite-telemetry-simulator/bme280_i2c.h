#ifdef __cplusplus
extern "C" {
#endif

#define BME_280_ADDR 0x76

#ifndef BME_280_I2C_H
#define BME_280_I2C_H
extern int32_t t_fine;

extern uint16_t dig_T1;
extern int16_t dig_T2, dig_T3;
extern uint16_t dig_P1;
extern int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
extern uint8_t dig_H1, dig_H3;
extern int8_t dig_H6;
extern int16_t dig_H2, dig_H4, dig_H5;

int32_t bme280_compensate_temp(int32_t adc_T);
uint32_t bme280_compensate_pressure(int32_t adc_P);
uint32_t bme280_compensate_humidity(int32_t adc_H);
void bme280_read_compensation_paramaters();
void bme280_init();
static void bme280_read_raw(int32_t *humidity, int32_t *pressure, int32_t *temperature);
int bme280_readings();
#endif  // BME_280_I2C_H

#ifdef __cplusplus
}
#endif