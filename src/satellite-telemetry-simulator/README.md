# MPU6050 sensor overview
The MPU6050 is a module with a 3-axis accelerometer and a 3-axis gyroscope. The gyroscope measures rotational velocity (rad/s). This is the change of the angular position over time along the X, Y, and Z-axis (roll, pitch, and yaw). This allows us to determine the orientation of an object. The accelerometer measures acceleration (rate of change of the object’s velocity). It senses static forces like gravity (9.8m/s<sup>2</sup>) or dynamic forces like vibrations or movement. The MPU6050 measures acceleration over the X, Y, and Z-axis.(9.8m/s<sup>2</sup>) or dynamic forces like vibrations or movement. 


<p align="center">
    <img src="https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2020/12/roll-pitch-yaw.png?resize=384%2C348&quality=100&strip=all&ssl=1" alt="MPU6050 axes">
</p> 


The MPU6050 measures acceleration over the X, Y, and Z-axis. This means the MPU6050 is the ideal sensor for simulating an orbital satellite.
## Sensor setup
1. Setup the I2C wire and pull up GPIO pins 4 & 5
2. Reset the sensor which will also clear sleep mode and then stabilise the sensor
3. Calibrate the sensor
    1. Place the sensor on a flat surface.
    2. Collect average readings over 5000 samples
    3. Calculate the average data for the accelerometer, gyroscope and the average temperature
    4. Calculate the offset values using this average data
    5. Write the offset values to the offset registers
4. Test the sensor
    1. Check the sensor is not in sleep mode, if it is then output error
    2. Configure the accelerometer and gyroscope range (done by default so no need to do anything here)
    3. Set up the self test bits inside the configure registers for the accelerometer and gyroscope
    4. Get the factory trim values for the gyroscope (values that affect the accuracy of the output that come from any issues in manufacturing/affected by the temperature of the chip or other factors)
    5. Calculate the factory trim values
    6. Read the raw values with self test enabled (in step 3) and store them into an array
    7. Reset the sensor to collect the raw values without self test enabled and save them into an array
    8. Check if factory trim values have been sucessfully collected, if not output error
    9. Scale the raw values for deviation calculation
    10. Check if the deviation values are within the accepted range, if so then the sensor has passed the test
5. Calibrate after testing
6. Read the data from the sensor
## Sensor wiring
| **MPU6050 Pin** | **Pico W Pin** |
|-----------------|----------------|
| VCC             | 3V3(OUT)       |
| GND             | GND            |
| SCL             | GPIO5          |
| SDA             | GPIO4          |
#### Sources
- [MPU6050 Datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
- [MPU6050 Register Map](https://www.arduino.cc/en/Guide/Mpu6050)
- [MPU6050 Arduino MicroPython tutorial](https://randomnerdtutorials.com/arduino-mpu-6050-accelerometer-gyroscope/)