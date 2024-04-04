/*
* Author:
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ls.h"

LS_t *allocLS(int n) {
    // Allocate memory for the LS_t structure.
    LS_t *ls = (LS_t*) malloc(sizeof(LS_t));

    // Allocate memory for the coefficient matrix (2D array).
    ls->A = (real_t**) malloc(n * sizeof(real_t*));
    for (int i = 0; i < n; ++i)
        ls->A[i] = (real_t*) calloc(n, sizeof(real_t));

    // Allocate memory for the constants array.
    ls->b = (real_t*) calloc(n, sizeof(real_t));

    // Set the size of the linear system.
    ls->n = n;

    return ls;
}

void freeLS(LS_t *ls) {
    // Free memory for the coefficient matrix.
    for (int i = 0; i < ls->n; ++i)
        free(ls->A[i]);
    free(ls->A);

    // Free memory for the constants array.
    free(ls->b);

    // Free memory for the LS_t structure.
    free(ls);
}

void readLS(LS_t *ls) {
    for (int i = 0; i < ls->n; ++i) {
        // Read coefficients of the equation.
        for (int j = 0; j < ls->n; ++j)
            scanf("%lf", &ls->A[i][j]);

        // Read the constants array.
        scanf("%lf", &ls->b[i]);
    }
}

void diagonalLS(LS_t *ls, real_t *d, int i, int j) {
    for (int k = 0; k + i < ls->n && k + j < ls->n; ++k)
        d[k] = ls->A[k + i][k + j];
}

void copyLS(LS_t *dest, LS_t *src) {
    // Copy coefficients matrix.
    for (int i = 0; i < src->n; ++i)
        memcpy(dest->A[i], src->A[i], src->n * sizeof(real_t));

    // Copy constants array.
    memcpy(dest->b, src->b, src->n * sizeof(real_t));

    // Set the size of the linear system.
    dest->n = src->n;
}

void printLS(LS_t *ls) {
     for (int i = 0; i < ls->n; ++i) {
        for (int j = 0; j < ls->n; ++j)
            printf(".12%lf ", ls->A[i][j]);

        printf("%.12lf\n", ls->b[i]);
    }
}

// Function to compute the residue array of a linear system.
// R = AX - B
void residueLS(LS_t *ls, real_t *x, real_t *r) {
    for (int i = 0; i < ls->n; ++i) {
        r[i] = 0.0;
        for (int j = 0; j < ls->n; ++j)
            r[i] += ls->A[i][j] * x[j];
    }

    for (int i = 0; i < ls->n; ++i)
        r[i] -= ls->b[i];
}

void printResolution(LS_t *ls, real_t *x, real_t *r) {
    // Compute the residue array.
    residueLS(ls, x, r);

    // Print the solution array.
    for (int i = 0; i < ls->n; ++i)
        printf("%.12f ", x[i]);
    printf("\n");

    // Print the residue array.
    for (int i = 0; i < ls->n; ++i)
        printf("%.12f ", r[i]);
    printf("\n");
}
