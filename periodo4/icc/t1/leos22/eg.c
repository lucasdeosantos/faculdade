#include "eg.h"

unsigned int encontraMax(double **A, int i) {
    return 1;
}

void trocaLinha(double **A, double *b, int i, unsigned int iPivo) {
    return;
}

void retrosSusbs(double **A, double *b, double *x, unsigned int n) {
    for (int i = n-1; i >= 0; --i) {
        x[i] = b[i];
        for (int j = i+1; j < n; ++j)
            x[i] -= A[i][j] * x[j];

        x[i] /= A[i][i];
    }
}

void eliminacaoGauss(double **A, double *b, unsigned int n) {
    for (int i = 0; i < n; ++i) {
        unsigned int iPivo = encontraMax(A, i);
        if (i != iPivo)
            trocaLinha(A, b, i, iPivo);

        for (int k = i+1; k < n; ++k) {
            double m = A[k][i] / A[i][i];
            A[k][i] = 0.0;
            for (int j = i+1; j < n; ++j)
                A[k][j] = A[i][j] * m;
            b[k] -= b[i] * m;
        }
    }
}

void eliminacaoGaussTridiagonal(double *d, double *a, double *c, double *b, double *x, unsigned int n) {
    for (int i = 0; i < n; ++i) {
        double m = a[i] / d[i];
        a[i] = 0.0;
        d[i+1] -= c[i] * m;
        b[i+1] -= b[i] * m;
    }
    
    x[n-1] = b[n-1] / d[n-1];
    for (int i = n-2; i >= 0; --i)
        x[i] = (b[i] - c[i] * x[i+1]) / d[i];
}
