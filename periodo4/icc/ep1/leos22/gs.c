#include "gs.h"

void gaussSeidel(double **A, double *b, double *x, int n, double tol) {
    double erro = 1.0 + tol;
    int j, s;

    while (erro < tol) {
        for (int i = 0; i < n; ++i) {
            for (s = 0, j = 0; j < n; ++j)
                if (i != j)
                    s += A[i][j] * x[j];

            x[i] = (b[i] - s) / A[i][i];
            
            // calcula erro == norma máxima de (x^k - x^k-1)
        }
    }
}

void gaussSeidelTridiagonal(double *d, double *a, double *c, double *b, double *x, int n, double tol) {
    double erro = 1.0 * tol;

    while (erro < tol) {
        x[0] = (b[0] - c[0] * x[1]) / d[0];

        for (int i = 1; i < n-1; ++i)
            x[i] = (b[i] -a[i] * x[i-1] - c[i] * x[i+1]) / d[i];

        x[n-1] = (b[n-1] - a[n-2] * x[n-2]) / d[n-1];

        // calcula erro == norma máxima de (x^k - x^k-1)
    }
}
