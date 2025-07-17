/*
 * Tema 2 ASC
 * 2024 Spring
 */
#include "utils.h"

/*
 * Add your unoptimized implementation here
 */

double* my_solver(int N, double *A, double* B) {
    // Allocate memory for the required matrices
    double *AtB = (double *)calloc(N * N, sizeof(double));
    double *BA = (double *)calloc(N * N, sizeof(double));
    double *C = (double *)calloc(N * N, sizeof(double));

    // Compute AtB = A_t * B
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k <= i; ++k) {
                AtB[i * N + j] += A[k * N + i] * B[k * N + j];
            }
        }
    }

    // Compute BA = B * A
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k <= j; ++k) {
                BA[i * N + j] += B[i * N + k] * A[k * N + j];
            }
        }
    }
    
    for (int i = 0; i < N * N; ++i) {
        AtB[i] += BA[i];
    }

    // Compute C = (AtB + BA) * B
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[i * N + j] += AtB[i * N + k] * B[j * N + k];
            }
        }
    }

    // Free allocated memory for intermediate matrices
    free(AtB);
    free(BA);

    return C;  // Caller is responsible for freeing this memory
}
