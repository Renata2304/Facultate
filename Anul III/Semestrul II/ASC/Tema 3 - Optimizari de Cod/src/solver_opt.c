/*
 * Tema 2 ASC
 * 2024 Spring
 */
#include "utils.h"

/*
 * Add your optimized implementation here
 */
double* my_solver(int N, double *A, double* B) {
	// Allocate memory for the required matrices
    double *AtB = (double *)malloc(N * N * sizeof(double));
    double *BA = (double *)malloc(N * N * sizeof(double));
    double *C = (double *)malloc(N * N * sizeof(double));

    // Compute AtB = A_t * B
    for (int i = 0; i < N; ++i) {
        register double *orig_pa = &A[i];
        for (int j = 0; j < N; ++j) {
            register double *pa = orig_pa;
            register double *pb = &B[j];
            register double suma = 0;
            for (int k = 0; k <= i; ++k) {
                suma += *pa * *pb;
                pa += N;
                pb += N;
            }
            AtB[i * N + j] = suma;
        }
    }

    // Compute BA = B * A
    for (int i = 0; i < N; i++) {
        register double *orig_pa = &B[i * N];
        for (int j = 0; j < N; j++) {
            register double *pa = orig_pa;
            register double *pb = &A[j];
            register double suma = 0;
            for (int k = 0; k <= j; k++) {
                suma += *pa * *pb;
                pa++;
                pb += N;
            }
            BA[i * N + j] = suma;
        }
    }
    
    for (int i = 0; i < N * N; ++i) {
        AtB[i] += BA[i];
    }

    // Compute C = (AtB + BA) * B using the provided pattern
    for (int i = 0; i < N; ++i) {
        register double *orig_pa = &AtB[i * N];
        for (int j = 0; j < N; ++j) {
            register double *pa = orig_pa;
            register double *pb = &B[j * N];
            register double suma = 0;
            for (int k = 0; k < N; ++k) {
                suma += *pa * *pb;
                pa++;
                pb++;
            }
            C[i * N + j] = suma;
        }
    }

    // Free allocated memory for intermediate matrices
    free(AtB);
    free(BA);

    return C;  // Caller is responsible for freeing this memory
}
