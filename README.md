> [!WARNING]
> This project is still in active development. It is not ready for release
# Satellite simulator app
A desktop app for simulating the way that telemetry data is collected from a orbital satellite and analysed/processed by ground stations. The simulators interface enables you to view a 3D model of the satellite by using the data from the accelerometer and gyroscope to map a 3D model. You can also send signals to the satellite the same way a real life ground station can (i.e. interrupt signals etc). The code for collecting the data from the sensors are calibrated to ensure the accuracy and consistency of the data using various filters such as a [kalman filter](https://en.wikipedia.org/wiki/Kalman_filter) to average and correct the data while still collecting data having the affect of the data becoming more accurate as it collects more data. The satellite (Pico W) communicates wirelessley with the laptop (ground station) using TCP and UDP over Wi-Fi.
## Filtering
To enable custom filtering and ensuring the data is as accurate as possible, there are several types of filtering involved. First both sensors code is written from scratch with the generated Pico SDK examples as a starting point and then a calabration process occurs automatically every time the app is started. These calibration values are then written to the registers of the sensors and then the offset values are calculated, adjusted and written back to the sensors to account for any noise coming from the sensors that is caused by any issues when the sensors were manufactured. Finally a Kalman filter is applied to both sensors to make sure the sensors are as accurate as possible by averaging and correct the data as more data is collected which makes the data more accurate as it goes on. A Kalman filter is a perfect candidate for a filtering algorithim in this project because the Kalmnan filter is used in all spaceflight, aerospace, IoT and computer vision projects. It was also used on the Apollo missions!
## Wireless communication over Wi-Fi
Ideally I would have done this part using an actual satellite communication protocol but that is not possible so I instead use UDP and TCP over Wi-Fi to communicate the data and then sockets to collect that data. Data is transmitted as a JSON object to ensure data is collected from the serial port and packaged reliably. 
## Signals and command protocol
There are various signals and commands you can send to the satellite to add an element of realism. These include:
- Shutdown signal
- Restart signal
- Calibrate satellite signal
- Change the telemetry transmission rate
- Toggle a system mode (low power or high power)
## The telemetry data
There are various data points collected:
- Acceleromter data on 3 axis
- Gyroscopic data on 3 axis
- MPU sensor temperature
- Enviromental temperature
- Altitude
- Barometric (atmospheric) pressure
- Humidity
- UTC time
- Pico W conditions
## 3D satellite model
Using the gyroscopic and accelerometer data a 3D model of a satellite is mapped and you can view it in 3 axis.
## The physical setup
The "satellite" is a Pico W connected to a MPU6050 accelerometer and gyroscopic sensor with a BME280 enviromental sensor. The telemetry data consists of the data from these sensors as well as the Pico W statistics and health such as temperature, available RAM, CPU usage etc. To see the specific hardware wirings please either see the docs or the `src/satellite-telemetry-simulator` directory.
## The build system
This project has got 2 CMake builds, one for the Qt app and one for the sensor code using the Pico SDK. This keeps the build systems seperate and reduces the complexity enabling you to easily build both projects and then run the entire project via the Qt app. To see more information please see the `src` directory. The good thing about this app is you only have to build the Qt app and then the Qt app handles the built process for the Pico SDK for you, the app also sends commands to use `Picotool` to automatically flash the UF2 file to the Pico W and then monitor the serial port.
## Docs
This project has pretty extensive and detailed documentation. Because it is such a complex project (requires you to have the same setup to clone this and run on your local machine) you can see diagrams and videos explaning this project, the build system, the setup and anything else needed.
