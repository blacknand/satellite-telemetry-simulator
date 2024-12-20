# MPU6050 sensor overview
The MPU6050 is a module with a 3-axis accelerometer and a 3-axis gyroscope. The gyroscope measures rotational velocity (rad/s). This is the change of the angular position over time along the X, Y, and Z-axis (roll, pitch, and yaw). This allows us to determine the orientation of an object. The accelerometer measures acceleration (rate of change of the object’s velocity). It senses static forces like gravity (9.8m/s<sup>2</sup>) or dynamic forces like vibrations or movement. The MPU6050 measures acceleration over the X, Y, and Z-axis.(9.8m/s<sup>2</sup>) or dynamic forces like vibrations or movement. 


<p align="center">
    <img src="https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2020/12/roll-pitch-yaw.png?resize=384%2C348&quality=100&strip=all&ssl=1" alt="MPU6050 axes">
</p> 


The MPU6050 measures acceleration over the X, Y, and Z-axis. This means the MPU6050 is the ideal sensor for simulating an orbital satellite.
## Sensor setup

#### Sources
- [MPU6050 Datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
- [MPU6050 Register Map](https://www.arduino.cc/en/Guide/Mpu6050)
- [MPU6050 Arduino MicroPython tutorial](https://randomnerdtutorials.com/arduino-mpu-6050-accelerometer-gyroscope/)