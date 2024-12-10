## Sensors
The Pico W ("Satellite") uses two senors, the MPU6050 gyroscopic sensor and the BME280 enviromental sensor.
### BME280 sensor data collection
- Collected every 10 seconds to mimic real world satellite enviromental data collection
### MPU6050 sensor data collection
- Collected every 100 ms to mimic real world satellite gyroscopic data collection, this is needed if a satellite needed to adjust itself in orbit etc.
