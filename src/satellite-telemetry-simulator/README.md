# MPU6050 sensor overview
The MPU6050 is a module with a 3-axis accelerometer and a 3-axis gyroscope. The gyroscope measures rotational velocity (rad/s). This is the change of the angular position over time along the X, Y, and Z-axis (roll, pitch, and yaw). This allows us to determine the orientation of an object. The accelerometer measures acceleration (rate of change of the object’s velocity). It senses static forces like gravity (9.8m/s<sup>2</sup>) or dynamic forces like vibrations or movement. The MPU6050 measures acceleration over the X, Y, and Z-axis.(9.8m/s<sup>2</sup>) or dynamic forces like vibrations or movement. 


<p align="center">
    <img src="https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2020/12/roll-pitch-yaw.png?resize=384%2C348&quality=100&strip=all&ssl=1" alt="MPU6050 axes">
</p> 


The MPU6050 measures acceleration over the X, Y, and Z-axis. This means the MPU6050 is the ideal sensor for simulating an orbital satellite.
## Sensor wiring
| **MPU6050 Pin** | **Pico W Pin** |
|-----------------|----------------|
| VCC             | 3V3(OUT)       |
| GND             | GND            |
| SCL             | GPIO5          |
| SDA             | GPIO4          |
| **BME280 Pin** | **Pico W Pin** |
|-----------------|----------------|
| VIN             | 3V3(OUT)       |
| GND             | GND            |
| SCK             | GPIO5          |
| SDI             | GPIO4          |
## Pico SDK setup
1. Copy `pico_sdk_import.cmake` into project directory
2. Set `PICO_SDK_PATH` to location of Pico SDK, probably in `~/.pico-sdk/pico-sdk`
3. Inlcude `pico_sdk_import.cmake` before cretating project inside of CMake list
## Building project
- Build this project via the Pico SDK VSCode extension, if you do not want to do it that way then just build like you would with any other CMake build project
### Pico SDK troubleshooting
- If the VSCode Pico extension becomes grayed out, it means VSCode cannot locate the SDK, this seems to be quite a rare issue but one I ran into a lot. If this happens, try the following:
    - Exectute:
        ```
        cd build
        rm -rf *
        cmake -DPICO_SDK_PATH=$PICO_SDK_PATH ..
        make
        ```
    If the output is correct then it means the extension should now work again, to check, try restarting VSCode or opening the command pallete `cmd` + `shift` + `p` and select `CMake: delete cache and reconfigure`. It is also worth noting you may get a notification asking you if you would like to import from the current project. If you get this notification then select yes and create which will enable the SDK extension again.
