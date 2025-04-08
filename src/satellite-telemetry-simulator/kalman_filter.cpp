#include "kalman_filter.h"

#include <string.h>
#include <cstdio>


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
    printf("B Init: %.2f, %.2f, %.2f\n", kf->B[5][0], kf->B[5][1], kf->B[5][2]); // Should be 0, 0, 1.00
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


void kalman_predict(KalmanState* state, KalmanFilter* kf, float accel[3]) {
    float adjusted_accel[3] = {accel[0], accel[1], accel[2] - 9.81f};
    printf("Predict Input: ax=%.2f, ay=%.2f, az=%.2f\n", accel[0], accel[1], accel[2]);
    float new_state[6];
    for (int i = 0; i < 6; i++) {
        new_state[i] = 0;
        for (int j = 0; j < 6; j++) {
            new_state[i] += kf->F[i][j] * state->state[j];
        }
        for (int j = 0; j < 3; j++) {
            new_state[i] += kf->B[i][j] * adjusted_accel[j];
        }
        printf("B[%d]: %.2f, %.2f, %.2f\n", i, kf->B[i][0], kf->B[i][1], kf->B[i][2]); // Check B
    }
    printf("New State Before: %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n",
           new_state[0], new_state[1], new_state[2],
           new_state[3], new_state[4], new_state[5]);
    memcpy(state->state, new_state, sizeof(new_state));
    printf("State After: %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n",
           state->state[0], state->state[1], state->state[2],
           state->state[3], state->state[4], state->state[5]);
    // P update unchanged
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


void kalman_update(KalmanState* state, KalmanFilter* kf, float measured_v[3]) {
    // y = z - H * x (error)
    float y[3];
    for (int i = 0; i < 3; i++) {
        y[i] = measured_v[i];
        for (int j = 0; j < 6; j++) {
            y[i] -= kf->H[i][j] * state->state[j];
        }
    }
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
    // x = x + K * y
    float new_state[6];
    for (int i = 0; i < 6; i++) {
        new_state[i] = state->state[i];
        for (int j = 0; j < 3; j++) {
            new_state[i] += K[i][j] * y[j];
        }
    }
    memcpy(state->state, new_state, sizeof(new_state));
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