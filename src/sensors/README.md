## Pico W setup
- Hold down BOOTSEL button and plug in Pico
- Choose `Raspberry Pi Pico W` and `UF2_board` port
- Upload will then fail: `No monitor available for the port protocol uf2conv. Could not connect to UF2_Board uf2conv port.`
- Now COM port will become available, select it and upload again. The program should now flash correctly

## Arduino IDE RP2040 setup
- Find the packages location for the RP2040
- Create `platform.local.txt`
- Add `-I` flag to `compiler.cpp.extra_flags=` to include boost library location
- If you run into relative path issue with nlohmann::json then add `json.hpp` location with `-I` flag

## MPU6050 Pico W sensor wiring
| MPU6050 Pin | Pico W Pin          |
|-------------|---------------------|
| VCC         | 3.3V (out) (pin 36) |
| GND         | GND                 |
| SCL         | GPIO 19 (pin 25)    |
| SDA         | GPIO 18 (pin 24)    |


## BME I2C Pico W wiring
- [Adafruit BME280](https://www.adafruit.com/product/2652) sensor uses address 0x77
- Adafruit BME280 sensor is configured for I2C connection


| BME280 Pin | Pico W Pin         |
|------------|--------------------|
| VIN        | 3.3V (out) (pin 36)|
| GND        | GND                |
| SCK        | GPIO 5 (pin 7)     |
| SDI        | GPIO 4 (pin 6)     |
