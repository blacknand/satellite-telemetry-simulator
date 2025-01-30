# Satellite simulator app
This is a macOS Qt app which provides a interface with a 3D model of an orbital satellite for monitoring and analysing the telemetry data coming from the satellite as well as the ability to send commands to the satellite. 
## Building both for the Qt app and the sensor code
Inside of `satellite_sim` and `satellite-telemetry-simulator` you will find `CMakeLists.txt` files for building both projects. Both projects need to be built on their own since the sensor code intefaces directly with the Pico via the Pico SDK since it provides functions to directly access and modify registers via I2C. Please see these directories for specific build instructions.
### `src` directory structure
- `satellite_sim` is the Qt app directory
- `satellite-telemetry-simulator` is the implemenetation of the hardware code for monitoring the sensors as well as getting the Pico W device data
- `common` is a non Pico SDK specific directory with useful classes used through out the whole project
- `data_preprocessing` contains the C++ files for handling the sensor data using JSON format
