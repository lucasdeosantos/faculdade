/*
* Author:
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#include <stdlib.h>
#include "gs.h"

int gaussSeidel(LS_t *ls, real_t tol) {
    real_t error = 1.0 + tol; // Initialize error to ensure the loop starts.
    int it = 0; // Iteration counter.

    // Continue iteration until error is below tolerance or maximum iterations reached.
    while (error > tol && it < MAXIT) {
        real_t maxError = 0.0; // Maximum error for this iteration.

        // Iterate over each equation in the system.
        for (int i = 0; i < ls->n; ++i) {
            real_t s = 0.0; // Variable to store the sum of the other variables multiplied by their coefficients.

            // Compute the sum of the other variables multiplied by their coefficients.
            for (int j = 0; j < ls->n; ++j)
                if (i != j)
                    s += ls->A[i][j] * ls->x[j];

            // Compute the new value of the variable based on the computed sum.
            real_t xi = (ls->b[i] - s) / ls->A[i][i];

            // Compute the absolute error for this variable and update the maximum error if needed.
            real_t absError = ABS(xi - ls->x[i]);
            if (absError > maxError)
                maxError = absError;

            // Update the value of the variable in the solution array.
            ls->x[i] = xi;
        }

        // Update the maximum error for this iteration.
        error = maxError;

        // Increment the iteration counter.
        it++;
    }

    // Return the number of iterations performed.
    return it;
}

int gaussSeidelTridiagonal(LS_t *ls, real_t tol) {
    // Allocate memory for the diagonals.
    real_t *d = (real_t*) malloc(ls->n * sizeof(real_t));
    real_t *a = (real_t*) malloc(ls->n * sizeof(real_t));
    real_t *c = (real_t*) malloc(ls->n * sizeof(real_t));

    // Extract diagonals from the coefficient matrix.
    diagonalLS(ls, d, 0, 0);
    diagonalLS(ls, a, 1, 0);
    diagonalLS(ls, c, 0, 1);

    real_t error = 1.0 + tol; // Initialize error to ensure the loop starts.
    int it = 0; // Iteration counter.

    // Continue iteration until error is below tolerance or maximum iterations reached.
    while (error > tol && it < MAXIT) {
        real_t maxError = 0.0; // Maximum error for this iteration.

        // Compute the first variable using the tridiagonal structure.
        ls->x[0] = (ls->b[0] - c[0] * ls->x[1]) / d[0];

        // Iterate over the remaining variables.
        for (int i = 1; i < ls->n - 1; ++i) {
            // Compute the new value of the variable using the tridiagonal structure.
            real_t xi = (ls->b[i] - a[i] * ls->x[i - 1] - c[i] * ls->x[i + 1]) / d[i];

            // Compute the absolute error for this variable and update the maximum error if needed.
            real_t absError = ABS(xi - ls->x[i]);
            if (absError > maxError)
                maxError = absError;

            // Update the value of the variable in the solution array.
            ls->x[i] = xi;
        }

        // Compute the last variable using the tridiagonal structure.
        ls->x[ls->n - 1] = (ls->b[ls->n - 1] - a[ls->n - 2] * ls->x[ls->n - 2]) / d[ls->n - 1];

        // Update the maximum error for this iteration.
        error = maxError;

        // Increment the iteration counter.
        it++;
    }

    // Free allocated memory.
    free(d);
    free(a);
    free(c);

    // Return the number of iterations performed.
    return it;
}
