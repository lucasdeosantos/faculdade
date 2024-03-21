#include "eg.h"
#include "sl.h"

int encontraMax(double **A, int i, int n) {
    double maior = A[i][0];
    int index = 0;

    for (int j = 1; j < n; ++j)
        if (A[i][j] > maior) {
            maior = A[i][j];
            index = j;
        }

    return index; 
}

void trocaLinha(double **A, double *b, unsigned int i, int iPivo, int n) {
    double *temp = alocarVetor(n);
    
    copiarVetor(A[i], temp, n);

    for (int j = 0; j < n; ++j) {
        A[i][j] = A[iPivo][j];
        A[iPivo][j] = temp[j];
    }

    double aux = b[i];
    b[i] = b[iPivo];
    b[iPivo] = aux;
}

void retrosSusbs(double **A, double *b, double *x, int n) {
    for (int i = n-1; i >= 0; --i) {
        x[i] = b[i];
        for (int j = i+1; j < n; ++j)
            x[i] -= A[i][j] * x[j];

        x[i] /= A[i][i];
    }
}

void eliminacaoGauss(double **A, double *b, int n) {
    for (int i = 0; i < n; ++i) {
        int iPivo = encontraMax(A, i, n);
        if (i != iPivo)
            trocaLinha(A, b, i, iPivo, n);

        for (int k = i+1; k < n; ++k) {
            double m = A[k][i] / A[i][i];
            A[k][i] = 0.0;
            for (int j = i+1; j < n; ++j)
                A[k][j] = A[i][j] * m;
            b[k] -= b[i] * m;
        }
    }
}

void eliminacaoGaussTridiagonal(double *d, double *a, double *c, double *b, double *x, int n) {
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
