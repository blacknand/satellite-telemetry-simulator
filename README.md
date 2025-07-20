> [!WARNING]
> This project is no longer being developed it. It is in a usable state, and the core system architecture is finished but it is ultimately a multi-year long passion project and there is 0 benefit to me continuing. Perhaps some time in the future I will finish it but more likely not.
# Satellite simulator app
macOS/Linux app that simulates the way a orbital satellite collects and relays telemetry data wirelessley. The project hardware consists of a Raspberry Pi Pico W, a MPU6050 MEMS device and a BME280 enviromental sensor.
## Roadmap
This project is actively under development, this roadmap lays out every feature currently implemented and every feature that I am planning to implement.
### ✅ Core System Architecture

- [x] Set up Raspberry Pi Pico W as the central microcontroller  
- [x] Integrate BME280 sensor for temperature, humidity, and pressure  
- [x] Integrate MPU6050 for gyroscope and accelerometer data  
- [x] Output raw sensor data at fixed intervals (e.g., every 100 ms)  
- [x] Format sensor data as JSON  
- [x] Send JSON data to the GUI for visualization  
- [x] Filter and display individual telemetry values

---

### ✅ Advanced Filtering & Data Processing

- [x] Implement basic Kalman filter for noisy sensor fusion  
- [ ] Tune Kalman filter parameters for real-time accuracy  
- [ ] Explore complementary filters as a comparison  
- [ ] Add quaternion or DCM-based orientation estimation (for 3D satellite rotation)  
- [ ] Implement anomaly detection (e.g., using thresholds or ML)  
- [ ] Add historical trend tracking (e.g., rolling averages, plotting over time)

---

### 🔧 Embedded Systems Engineering

- [x] Multi-threaded sensor reading and data transmission  
- [ ] Add basic command handling (receive commands from base station/GUI)  
- [ ] Log telemetry data to SD card or external storage (optional)  
- [ ] Implement power and thermal management simulation (mock or real)  
- [ ] Output Pico system status: CPU temp, memory usage, uptime  

---

### 💻 Qt GUI Interface

- [x] Create GUI to display live telemetry values  
- [x] Format GUI with SpaceX-style theme  
- [ ] Add 2D/3D satellite visualization (orientation from MPU6050)  
- [ ] Add live charts for sensor data (temperature, orientation, etc.)  
- [ ] Build a command interface to send instructions to the Pico  
- [ ] Add simulation start/stop/reset buttons  
- [ ] Create error or warning overlays (e.g., when a value is out of range)

---

### 📡 Communication Layer

- [ ] Enable wireless communication between Pico W and laptop (ESP-NOW, MQTT, or TCP)  
- [ ] Implement command+response protocol between ground station and satellite  
- [ ] Add simulated latency, packet loss or corruption (for realism)  
- [ ] Encrypt telemetry and command channels (TLS or basic XOR as a placeholder)

---

### 🧠 FPGA Integration

- [ ] Port Kalman filter to HDL (VHDL/Verilog) for testbench use  
- [ ] Simulate real-time telemetry stream into FPGA  
- [ ] Develop Pico-compatible SPI or UART HDL module  
- [ ] Demonstrate FPGA acting as telemetry decoder or command relay  
- [ ] Optimize filtering or sensor processing with HLS tools (e.g., Xilinx Vivado HLS)

---

### 🧪 Testing & Reliability

- [ ] Add unit tests for each telemetry parsing and filtering module  
- [ ] Add GUI integration tests (e.g., simulate incoming data)  
- [ ] Create test suite with recorded sensor data playback  
- [ ] Implement data validation and CRC error checking  
- [ ] Track uptime and unexpected resets of the Pico system

---

### 🎯 UX Polish & Deployment

- [ ] Package the entire Qt GUI into a cross-platform app  
- [ ] Automate Pico firmware uploads using `picotool` on app startup  
- [ ] Provide user-friendly error messages and fallback mechanisms  
- [ ] Create README, user manual, and demo video  
- [ ] Add simulation presets (e.g., satellite in orbit, spin-stabilized, tumbling)

---

### 📁 Optional Expansion Ideas

- [ ] Add GPS module (simulate real satellite movement)  
- [ ] Use ROS2 for more complex satellite-ground station messaging  
- [ ] Add support for CAN bus protocol 

---
## Setup/building
See the [`docs`](https://github.com/blacknand/satellite-telemetry-simulator/tree/main/docs) for the physical sensor and Pico setup. The project consists of two seperate CMake builds, to keep the embedded systems side and the app side seperate.
### Building for the Pico SDK
The embedded systems/hardware site of the project uses the Pico SDK for low level memory access and manipulation as well as extreme efficiency and speed gains. The easiest way to build therefore is to use the VSCode Pico extension. To enable this, do the following:
1. Copy `pico_sdk_import.cmake` into project directory
2. Set `PICO_SDK_PATH` to location of Pico SDK, probably in `~/.pico-sdk/pico-sdk`
3. Inlcude `pico_sdk_import.cmake` before cretating project inside of CMake list


Should you encounter any errors, try to first build the project manually rather than using the VSCode extension.
- To build for the Pico:
```bash
cd build
rm -rf *
cmake -DPICO_SDK_PATH=$PICO_SDK_PATH ..
make
```

### Building for the Qt app
The app is a Qt app, which is slightly simpler to build for:
- Set `CMAKE_PREFIX_PATH` to point to Qt installation

    ```bash
    cmake -B build -S . -DCMAKE_PREFIX_PATH=/Users/<your username>/Qt/6.8.1/macos
    ```

- Build

    ```bash
    cmake --build build
    ```

- Run 

    ```bash
    ./build/satellite_sim.app/Contents/MacOS/satellite_sim
