/*
 * Tema 2 ASC
 * 2024 Spring
 */
#include "utils.h"
#include <cblas.h>

/* 
 * Add your BLAS implementation here
 */
double* my_solver(int N, double *A, double *B) {
    double* AtB = (double *)calloc(N * N, sizeof(double));
    double* BA = (double *)calloc(N * N, sizeof(double));
    double* C = (double *)calloc(N * N, sizeof(double));

    // Copies B in AtB
    cblas_dcopy(N * N, B, 1, AtB, 1);
    // Copies B in BA
    cblas_dcopy(N * N, B, 1, BA, 1);

    // Calculate AtB = A^T * B using A as an upper triangular matrix
    cblas_dtrmm(CblasRowMajor, CblasLeft, CblasUpper, CblasTrans, CblasNonUnit, N, N, 1.0, A, N, AtB, N);

    // Calculate BA = B * A using A as an upper triangular matrix
    cblas_dtrmm(CblasRowMajor, CblasRight, CblasUpper, CblasNoTrans, CblasNonUnit, N, N, 1.0, A, N, BA, N);

    // Add AtB and BA to get intermediate result
    cblas_daxpy(N * N, 1.0, AtB, 1, BA, 1);

    // Calculate C = (AtB + BA) * B^T
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, N, N, N, 1.0, BA, N, B, N, 0.0, C, N);

    free(AtB);
    free(BA);

    return C;
}