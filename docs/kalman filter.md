## What is the Kalman filter doing?
The Kalman filter acts like a smart assistant that:
1. Guesses: "Based on where the satellite was and how fast it was going, I think it may be here now".
2. Checks: "Sensor output is something different, I will compare".
3. Blends: "I will mix my guess with the sensor data, trusting each one based on how reliable it seems".
It repeats this time each time the Pico (Satellite) gets new data (i.e. with `telemetry_delay = n`). 
For the 6 states--position (px, py, pz) and velocity (vx, vy, vz)--it's guessing and correcting all six numbers at once, using accel (m/s^2) to drive the motion.
All 6 states come from the MPU6050 IMU sensor/sensor for the accelerometer and gyroscope.


The filter is used to estimate both the position and the velocity of an object in three dimensions (x, y and z). This works by using a six-element state vector and several matricies to update the estimation over time.


**Two steps**
- **Predict**: Uses physics (e.g. velocity changes with accel, position changes with velocity) to predict the next state.
- **Update**: Adjusts the guess when you get a sensor reading, balancing trust between the guess and the noisy data.

## Core concepts
To understand the core concepts, a few variables/concepts are required:
### The state vector
Holds our best guess as to where the satellite is and how fast it is moving. The state vector is a 6-element array defined as:
- **Positions**: `px`, `py`, `pz` for the position in x, y and z directions
- **Velocities**: `vx`, `vy`, `vz` for the speed in the corresponding directions 


The state vector is store in the `state` field of the `KalmanState` struct. See [`kalman_filter.h`](https://github.com/blacknand/satellite-telemetry-simulator/blob/main/src/satellite-telemetry-simulator/kalman_filter.h)


### The Covariance matrix
A 6 * 6 table that represents our uncertianty about each element of the state. A high number (i.e. 1000) means we are very uncertian about that part of the state. At initialisation, every diagonal element is set to 10000 to indicate that the filter starts with a very uncertian estimate:
```bash
for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
        state->P[i][j] = (i == j) ? 1000.0f : 0.0f;
    }
}
```
### Other notes
- `dt`: the time step, a small number (in seconds) that represents how much time passes between one states estimate and the next.
- `^ T`: transpose, when writing a vector as a row for convinence the transpose tells us to flip it into a column.
### The state vector
The "state" is what is being esitmated:


```bash
x = [Px, Py, Pz, Vx, Vy, Yz] ^ T
```
where:


- `Px, Py, Pz` are the position in meters (x, y, z axes).
- `Vx, Vy, Vz` are the velocity in meters per second (x, y, z axes).
- The size is a 6 * 1 matrix for 6 rows and 1 column


### Key matricies
**1. F (State transition matrix):**  
A 6 * 6 matrix that describes how the state evolves over time without external inputs (such as acceleration). Esentially F predicts how the state changes from one time step to the next if nothing unexpected happens.
```bash
float F_init[6][6] = {
    {1, 0, 0, dt, 0, 0},
    {0, 1, 0, 0, dt, 0},
    {0, 0, 1, 0, 0, dt},
    {0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1}
};
```
The way the matrix is set up is it uses the time step `dt` to update positions using current velocities. For example, it has entires where positon at the next step equals the current position + `dt` (i.e. `dt` * velocity). If you have an object moving at a constant speed, after time `dt` its new position is:
```bash
new position = old position + velocity * dt
```
This rule is captured inside of the top three rows of F.


**2. B (Control input matrix):**  
A 6 * 3 matrix that maps external inputs (`acceleration u = [a ^ x, a ^ y, a ^ z]`) to the state.
```bash
float B_init[6][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {dt, 0, 0},
    {0, dt, 0},
    {0, 0, dt}
};
```
It connects acceleration (the external input(s)) to changes in velocity. Notice that only the velocity part of the state (the bottom 3 rows) is directly affected (multiplied by `dt`). The positions are not directly affected by acceleration in this matrix, they change indirectly via velocity.


**3. H (Measurement matrix):**  
A 3 * 6 matrix that relates the state to what is measured. Esentially maps the full state down to what we can actually measure with our sensors.
```bash
float H_init[3][6] = {
    {0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1}
};
```
H only "sees" the velocity part of the state--meaning the filter's corrections are based on velocity measurements. H picks out velocities from the state vector.

**4. Q (Process noise covariance):**  
A 6 * 6 matrix that represents uncertainty in the model (i.e., random jolts or model errors).
```bash
for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
        kf->Q[i][j] = (i == j) ? ((i < 3) ? 0.01f : 0.1f) : 0.0f;
    }
}
```
Diagonal values for the positions are set to a small number (0.01) and for the velocities a slightly larger number (0.1). This reflects that we expect more uncertainty in how velocities change compared to positons.

**5. R (Measurement noise covariance):**  
A 3 * 3 matrix that represents noises in the sensor measurements.
```bash
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        kf->R[i][j] = (i == j) ? 0.1f : 0.0f;
    }
}
```
It is setup to use a constant moderate noise value (0.1) along its diagonal for the velocity measurements.

**6. P (State covariance matrix):**  
A 6 * 6 matrix that tracks uncertainty in the state estimate. Starts high and shrinks as the model gains confidence.
```bash
for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
        state->P[i][j] = (i == j) ? 1000.0f : 0.0f;
    }
}
```
