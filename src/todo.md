# 🚀 Satellite Telemetry Simulator - Project Goals

## ✅ Current Progress
- Successfully output sensor data to the GUI.
- Converted sensor data into a JSON object and filtered individual values for display.

## 🎯 Remaining Goals

### 1️⃣ Command & Signal System
- Implement a **command protocol** for sending signals to the satellite (Pico).
- Commands may include:
  - Change telemetry rate.
  - Toggle system modes (e.g., low-power mode).
  - Reset system.

### 2️⃣ Wireless Communication
- Establish **wireless data transfer** between the laptop and Pico.
- Test this **last** since the Pico is damaged but simulate it first.
- Options: **Wi-Fi (TCP/UDP), Bluetooth, or USB Serial**.

### 3️⃣ Automated UF2 Uploading
- When starting the app, **automatically flash the Pico with a UF2 file** via **Picotool**.
- This removes the need for manual flashing.

### 4️⃣ Data Filtering for Accuracy
- Apply a **Kalman filter** (or alternative) to sensor data.
- This will smooth out noisy telemetry readings.

### 5️⃣ System Status Monitoring
- Output Pico **system status** such as:
  - CPU Temperature
  - Memory usage
  - Uptime
  - Battery voltage (if applicable).

### 6️⃣ GUI Enhancements
- Design the **GUI with a SpaceX-style theme**.
- Improve layout, fonts, and color scheme.

### 7️⃣ 3D Satellite Visualization
- Implement a **3D model of a satellite** that rotates based on telemetry data.
- **Qt 3D** or **OpenGL** will be used for rendering.
- Fetch **real-time roll, pitch, and yaw** from MPU6050 IMU.

---

## 🔄 Suggested Order for Implementation

### **Phase 1: Core Functionality**
1. **Command System** – Enables bidirectional control.
2. **System Status Monitoring** – Useful for debugging.
3. **Data Filtering (Kalman Filter)** – Improves accuracy.

### **Phase 2: Advanced Features**
4. **Wireless Communication** – Adds realism (mocked first, tested last).
5. **Automated UF2 Uploading** – Streamlines workflow.

### **Phase 3: Visual & UI Enhancements**
6. **GUI Enhancements** – Improves UX with a polished SpaceX theme.
7. **3D Satellite Visualization** – Most complex but highly impressive.

---

## 🛠️ Next Steps
- Start with **command system implementation**.
- Define a basic **message format** for sending commands.
- Would you like a **template for the command protocol**?
