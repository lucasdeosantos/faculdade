#include <math.h>
#include <stdlib.h>
#include "gs.h"

int gaussSeidel(LS_t *ls, double tol) {
    double error = 1.0 + tol;
    int it = 0;

    while (error > tol && it < MAXIT) {
        double maxError = 0.0;
        for (int i = 0; i < ls->n; ++i) {
            double s = 0.0;
            for (int j = 0; j < ls->n; ++j)
                if (i != j)
                    s += ls->A[i][j] * ls->x[j];

            double xi = (ls->b[i] - s) / ls->A[i][i];

            double absError = fabs(xi - ls->x[i]);
            if (absError > maxError)
                maxError = absError;

            ls->x[i] = xi;
        }
        error = maxError;
        it++;
    }
    return it;
}

int gaussSeidelTridiagonal(LS_t *ls, double tol) {
    real_t *d = (real_t*) malloc(ls->n * sizeof(real_t));
    real_t *a = (real_t*) malloc(ls->n * sizeof(real_t));
    real_t *c = (real_t*) malloc(ls->n * sizeof(real_t));

    diagonalLS(ls, d, 0, 0);
    diagonalLS(ls, a, 1, 0);
    diagonalLS(ls, c, 0, 1);

    double error = 1.0 + tol;
    int it = 0;

    while (error > tol && it < MAXIT) {
        double maxError = 0.0;
        ls->x[0] = (ls->b[0] - c[0] * ls->x[1]) / d[0];

        double xi;
        for (int i = 1; i < ls->n - 1; ++i) {
            xi = (ls->b[i] - a[i] * ls->x[i - 1] - c[i] * ls->x[i + 1]) / d[i];

            double absError = fabs(xi - ls->x[i]);
            if (absError > maxError)
                maxError = absError;

            ls->x[i] = xi;
        }
        ls->x[ls->n - 1] = (ls->b[ls->n - 1] - a[ls->n - 2] * ls->x[ls->n - 2]) / d[ls->n - 1];

        error = maxError;
        it++;
    }

    free(d);
    free(a);
    free(c);

    return it;
}
