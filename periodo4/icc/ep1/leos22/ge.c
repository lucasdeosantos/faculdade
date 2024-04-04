/*
* Author:
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#include <string.h>
#include <stdlib.h>
#include "ge.h"

// Function to find the row with the maximum value in a given column.
uint_t findMax(LS_t *ls, uint_t i)
{
    real_t max = ls->A[i][i];
    uint_t index = i;

    for (uint_t j = i + 1; j < ls->n; ++j)
        if (ls->A[j][i] > max) {
            max = ls->A[j][i];
            index = j;
        }

    return index;
}

// Function to swap two rows in the coefficient matrix and the constants array.
void lineSwap(LS_t *ls, uint_t i, uint_t iPivo)
{
    real_t *temp = (real_t *) malloc(ls->n * sizeof(real_t));

    // Swap rows in the coefficient matrix.
    memcpy(temp, ls->A[i], ls->n * sizeof(real_t));
    memcpy(ls->A[i], ls->A[iPivo], ls->n * sizeof(real_t));
    memcpy(ls->A[iPivo], temp, ls->n * sizeof(real_t));

    // Swap elements in the constants array.
    real_t aux = ls->b[i];
    ls->b[i] = ls->b[iPivo];
    ls->b[iPivo] = aux;

    free(temp);
}

// Function to perform back substitution to solve the system of equations.
void retrosSusbs(LS_t *ls, real_t *x)
{
    for (int_t i = int_t(ls->n) - 1; i >= 0; --i) {
        x[i] = ls->b[i];
        for (uint_t j = i + 1; j < ls->n; ++j)
            x[i] -= ls->A[i][j] * x[j];

        x[i] /= ls->A[i][i];
    }
}

void gaussElimination(LS_t *ls, real_t *x)
{
    for (uint_t i = 0; i < ls->n; ++i) {
        // Find the row with the maximum value in the current column.
        uint_t iPivo = findMax(ls, i);
        if (i != iPivo)
           lineSwap(ls, i, iPivo);

        // Perform elimination.
        for (uint_t k = i + 1; k < ls->n; ++k) {
            real_t m = ls->A[k][i] / ls->A[i][i];
            ls->A[k][i] = 0.0;
            for (uint_t j = i + 1; j < ls->n; ++j)
                ls->A[k][j] -= ls->A[i][j] * m;
            ls->b[k] -= ls->b[i] * m;
        }
    }
    // Perform back substitution.
    retrosSusbs(ls, x);
}

void gaussTridiagonalElimination(LS_t *ls, real_t *x)
{
    // Allocate memory for the diagonals.
    real_t *d = (real_t *) malloc(ls->n * sizeof(real_t));
    real_t *a = (real_t *) malloc(ls->n * sizeof(real_t));
    real_t *c = (real_t *) malloc(ls->n * sizeof(real_t));

    // Extract diagonals from the coefficient matrix.
    diagonalLS(ls, d, 0, 0);
    diagonalLS(ls, a, 1, 0);
    diagonalLS(ls, c, 0, 1);

    // Forward elimination.
    for (uint_t i = 0; i < ls->n - 1; ++i) {
        real_t m = a[i] / d[i];
        a[i] = 0.0;
        d[i + 1] -= c[i] * m;
        ls->b[i + 1] -= ls->b[i] * m;
    }

    // Back substitution.
    x[ls->n - 1] = ls->b[ls->n - 1] / d[ls->n - 1];
    for (int_t i = int_t(ls->n) - 2; i >= 0; --i)
        x[i] = (ls->b[i] - c[i] * x[i + 1]) / d[i];

    // Free allocated memory.
    free(d);
    free(a);
    free(c);
}
