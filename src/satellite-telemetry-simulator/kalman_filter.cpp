#include "kalman_filter.h"

#include <string.h>
#include <cstdio>

// TODO: Look into using a matrix library rather than hardcoding


KalmanState kf_state;
KalmanFilter kf;
bool kf_initialised = false;


void init_kalman(KalmanState* state, KalmanFilter* kf, float dt) {
    // Zero state (start at rest)
    for (int i = 0; i < 6; i++) {
        state->state[i] = 0.0f;
    }

    // High initial uncertainty (P diagonal = 1000)
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            state->P[i][j] = (i == j) ? 1000.0f : 0.0f;
        }
    }

    // F matrix (position uses velocity)
    float F_init[6][6] = {
        {1, 0, 0, dt, 0, 0},
        {0, 1, 0, 0, dt, 0},
        {0, 0, 1, 0, 0, dt},
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 1}
    };
    memcpy(kf->F, F_init, sizeof(F_init));

    // B matrix (accel affects velocity)
    float B_init[6][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {dt, 0, 0},
        {0, dt, 0},
        {0, 0, dt}
    };
    memcpy(kf->B, B_init, sizeof(B_init));

    // H matrix (measure velocities)
    float H_init[3][6] = {
        {0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 1}
    };
    memcpy(kf->H, H_init, sizeof(H_init));

    // Q (process noise, small)
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            kf->Q[i][j] = (i == j) ? ((i < 3) ? 0.01f : 0.1f) : 0.0f;
        }
    }

    // R (measurement noise, moderate)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            kf->R[i][j] = (i == j) ? 0.1f : 0.0f;
        }
    }
}


// Predict the new state based on the previous state and the external inputs (acceleration)
void kalman_predict(KalmanState* state, KalmanFilter* kf, float accel[3]) {
    /**
     * Each element of the new state is calculated by:
     *      multiplying the current state by the corresponding element in F,
     *      adding the effect of acceleration through B,
     *      looping over the elements using nested loops.
     * The code then calculates a new covariance by first multiplying F with the previous
     * covariance, then with the transpose of F, and finally adding the process nosie Q.
     */

    // The given acceleration measurement is adjusted to remove the effect of gravity (9.81 m/s ^ 2)
    // because the sensor measures gravitational acceleration, but we want the net acceleration affecting the satellite's movement
    float adjusted_accel[3] = {accel[0], accel[1], accel[2] - 9.81f};  
    float new_state[6];

    for (int i = 0; i < 6; i++) {
        // new state = F * current state + B * adjusted acceleration
        new_state[i] = 0;
        for (int j = 0; j < 6; j++) {
            new_state[i] += kf->F[i][j] * state->state[j];
        }

        for (int j = 0; j < 3; j++) {
            new_state[i] += kf->B[i][j] * adjusted_accel[j];
        }
        printf("B[%d]: %.2f, %.2f, %.2f\n", i, kf->B[i][0], kf->B[i][1], kf->B[i][2]); // Check B
    }

    memcpy(state->state, new_state, sizeof(new_state));

    // P update unchanged
    // Updating the covariance: P = F * P * F ^ T + Q
    // the update that our uncertainty changes when we predict, but the noise process (Q)
    // is added to account for any model error 
    float temp[6][6], new_P[6][6];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            temp[i][j] = 0;
            for (int k = 0; k < 6; k++) {
                temp[i][j] += kf->F[i][k] * state->P[k][j];
            }
        }
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            new_P[i][j] = 0;
            for (int k = 0; k < 6; k++) {
                new_P[i][j] += temp[i][k] * kf->F[j][k];
            }
            new_P[i][j] += kf->Q[i][j];
        }
    }

    memcpy(state->P, new_P, sizeof(new_P));
}


// Correct the predicted state with an actual measurement (velocity measurement)
void kalman_update(KalmanState* state, KalmanFilter* kf, float measured_v[3]) {
    // The error tells you how far off the prediction is from the actual sensor value
    // y = z - H * x (error)
    float y[3];
    for (int i = 0; i < 3; i++) {
        y[i] = measured_v[i];
        for (int j = 0; j < 6; j++) {
            y[i] -= kf->H[i][j] * state->state[j];
        }
    }

    // Gives an idea of the combined uncertainty from both the prediction (P) and the measurement noise (R)
    // S = H * P * H^T + R
    float HP[3][6], S[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++) {
            HP[i][j] = 0;
            for (int k = 0; k < 6; k++) {
                HP[i][j] += kf->H[i][k] * state->P[k][j];
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            S[i][j] = kf->R[i][j];
            for (int k = 0; k < 6; k++) {
                S[i][j] += HP[i][k] * kf->H[j][k]; // H^T
            }
        }
    }

    // Determine the Kalman grain, because S is assumed diagonal (each measurement is independent),
    // the inverse is approximated simply by dividing by each diagonal element.
    // The Kalman grain tells you how much to trust the measurement vs your prediction
    // K = P * H^T * S^-1 (simplified: assume S diagonal, invert manually)
    float K[6][3];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 3; j++) {
            K[i][j] = 0;
            for (int k = 0; k < 6; k++) {
                K[i][j] += state->P[i][k] * kf->H[j][k]; // H^T
            }
            K[i][j] /= S[j][j]; // Approx inverse
        }
    }

    // Update the state: adjust the prediction by a fraction of the error
    // x = x + K * y
    float new_state[6];
    for (int i = 0; i < 6; i++) {
        new_state[i] = state->state[i];
        for (int j = 0; j < 3; j++) {
            new_state[i] += K[i][j] * y[j];
        }
    }

    memcpy(state->state, new_state, sizeof(new_state));

    // Update the covariance matrix to reflect that after incorporating the measurement,
    // you are more confident in the new state
    // P = (I - K * H) * P
    float KH[6][6], temp[6][6], new_P[6][6];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            KH[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                KH[i][j] += K[i][k] * kf->H[k][j];
            }
            temp[i][j] = (i == j) ? 1.0f : 0.0f;
            temp[i][j] -= KH[i][j];
        }
    }
    
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            new_P[i][j] = 0;
            for (int k = 0; k < 6; k++) {
                new_P[i][j] += temp[i][k] * state->P[k][j];
            }
        }
    }

    memcpy(state->P, new_P, sizeof(new_P));
}