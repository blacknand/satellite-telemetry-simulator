## MPU6050 Pico W sensor wiring
| MPU6050 Pin | Pico W Pin          |
|-------------|---------------------|
| VCC         | 3.3V (out) (pin 36) |
| GND         | GND                 |
| SCL         | GPIO 21 (pin 27)    |
| SDA         | GPIO 20 (pin 26)    |


## BME I2C Pico W wiring
- [Adafruit BME280](https://www.adafruit.com/product/2652) sensor uses address 0x77
- Adafruit BME280 sensor is configured for I2C connection


| BME280 Pin | Pico W Pin         |
|------------|--------------------|
| VIN        | 3.3V (out) (pin 36)|
| GND        | GND                |
| SCK        | GPIO 5 (pin 7)     |
| SDI        | GPIO 4 (pin 6)     |
