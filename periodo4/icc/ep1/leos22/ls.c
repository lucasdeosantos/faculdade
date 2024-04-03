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

    // Allocate memory for the constants, variables and residue array.
    ls->b = (real_t*) calloc(n, sizeof(real_t));
    ls->x = (real_t*) calloc(n, sizeof(real_t));
    ls->r = (real_t*) calloc(n, sizeof(real_t));

    // Set the size of the linear system.
    ls->n = n;

    return ls;
}

void freeLS(LS_t *ls) {
    // Free memory for the coefficient matrix.
    for (int i = 0; i < ls->n; ++i)
        free(ls->A[i]);
    free(ls->A);

    // Free memory for the constants, variables, and residue arrays.
    free(ls->b);
    free(ls->x);
    free(ls->r);

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
    for(int k = 0; k < ls->n && k + i < ls->n && k + j < ls->n; ++k)
        d[k] = ls->A[k + i][k + j];
}

void copyLS(LS_t *dest, LS_t *src) {
    // Copy coefficients, constants, variables and residue arrays.
    for(int i = 0; i < src->n; ++i)
        memcpy(dest->A[i], src->A[i], src->n * sizeof(real_t));

    memcpy(dest->b, src->b, src->n * sizeof(real_t));
    memcpy(dest->x, src->x, src->n * sizeof(real_t));
    memcpy(dest->r, src->r, src->n * sizeof(real_t));

    // Set the size of the linear system.
    dest->n = src->n;
}

void printLS(LS_t *ls) {
     for (int i = 0; i < ls->n; ++i) {
        for (int j = 0; j < ls->n; ++j)
            printf("%lf ", ls->A[i][j]);

        printf("%lf\n", ls->b[i]);
    }
}

// Function to compute the residue array of a linear system.
// R = AX - B
void residueLS(LS_t *ls) {
    for (int i = 0; i < ls->n; ++i) {
        ls->r[i] = 0.0;
        for (int j = 0; j < ls->n; ++j)
            ls->r[i] += ls->A[i][j] * ls->x[j];
    }

    for (int i = 0; i < ls->n; ++i)
        ls->r[i] -= ls->b[i];
}

void printResolution(LS_t *ls) {
    // Compute the residue array.
    residueLS(ls);

    // Print the solution array.
    for (int i = 0; i < ls->n; ++i)
        printf("%.12f ", ls->x[i]);
    printf("\n");

    // Print the residue array.
    for (int i = 0; i < ls->n; ++i)
        printf("%.12f ", ls->r[i]);
    printf("\n");
}
