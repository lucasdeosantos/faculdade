#include <math.h>
#include "gs.h"
#include "sl.h"

int gaussSeidel(double **A, double *b, double *x, int n, double tol) {
    double erro;
    int it = 0;

    inicializarVetor(x, n);

    while (it < MAXIT) {
        double erroMax = 0.0;
        for (int i = 0; i < n; ++i) {
            double s = 0.0;
            for (int j = 0; j < n; ++j)
                if (i != j)
                    s += A[i][j] * x[j];

            double xi = (b[i] - s) / A[i][i];

            double erroAbs = fabs(xi - x[i]);
            if (erroAbs > erroMax)
                erroMax = erroAbs;

            x[i] = xi;
        }
        erro = erroMax;

        if (erro < tol)
            break;

        it++;
    }
    return it;
}

int gaussSeidelTridiagonal(double *d, double *a, double *c, double *b, double *x, int n, double tol) {
    double erro;
    int it = 0;

    inicializarVetor(x, n);

    while (it < MAXIT) {
        double erroMax = 0.0;
        x[0] = (b[0] - c[0] * x[1]) / d[0];

        double xi;
        for (int i = 1; i < n-1; ++i) {
            xi = (b[i] -a[i] * x[i-1] - c[i] * x[i+1]) / d[i];

            double erroAbs = fabs(xi - x[i]);
            if (erroAbs > erroMax)
                erroMax = erroAbs;

            x[i] = xi;
        }
        x[n-1] = (b[n-1] - a[n-2] * x[n-2]) / d[n-1];

        erro = erroMax;

        if (erro < tol)
            break;

        it++;
    }
    return it;
}
