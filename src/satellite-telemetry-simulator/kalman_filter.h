#ifdef __cplusplus
extern "C" {
#endif

#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#pragma once


typedef struct {
    float state[6]; // [px, py, pz, vx, vy, vz]
    float P[6][6];  // Covariance matrix
} KalmanState;


typedef struct {
    float F[6][6];  // State transition
    float B[6][3];  // Control matrix
    float H[3][6];  // Measurement matrix
    float Q[6][6];  // Process noise
    float R[3][3];  // Measurement noise
} KalmanFilter;


extern KalmanState kf_state;
extern KalmanFilter kf;
extern bool kf_initialised;

void init_kalman(KalmanState* state, KalmanFilter* kf, float dt);
void kalman_predict(KalmanState* state, KalmanFilter* kf, float accel[3]);
void kalman_update(KalmanState* state, KalmanFilter* kf, float measured_v[3]);


#endif  // KALMAN_FILTER_H

#ifdef __cplusplus
}
#endif