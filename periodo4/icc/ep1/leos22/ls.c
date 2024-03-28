#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ls.h"

LS_t *allocLS(int n) {
    LS_t *ls = (LS_t*) malloc(sizeof(LS_t));

    ls->A = (real_t**) malloc(n * sizeof(real_t*));
    for (int i = 0; i < n; ++i)
        ls->A[i] = (real_t*) malloc(n * sizeof(real_t));

    ls->b = (real_t*) malloc(n * sizeof(real_t));
    ls->x = (real_t*) calloc(n, sizeof(real_t));
    ls->r = (real_t*) calloc(n, sizeof(real_t));
    ls->n = n;

    return ls;
}

void freeLS(LS_t *ls) {
    for (int i = 0; i < ls->n; ++i)
        free(ls->A[i]);
    free(ls->A);

    free(ls->b);
    free(ls->x);
    free(ls->r);

    free(ls);
}

void readLS(LS_t *ls) {
    for (int i = 0; i < ls->n; ++i) {
        for (int j = 0; j < ls->n; ++j)
            scanf("%lf", &ls->A[i][j]);

        scanf("%lf", &ls->b[i]);
    }
}

void diagonalLS(LS_t *ls, real_t *d, int i, int j) {
    for(int k = 0; k < ls->n && k + i < ls->n && k + j < ls->n; ++k)
        d[k] = ls->A[k + i][k + j];
}

void copyLS(LS_t *dest, LS_t *src) {
    for(int i = 0; i < src->n; ++i)
        memcpy(dest->A[i], src->A[i], src->n * sizeof(real_t));

    memcpy(dest->b, src->b, src->n * sizeof(real_t));
    memcpy(dest->x, src->x, src->n * sizeof(real_t));
    memcpy(dest->r, src->r, src->n * sizeof(real_t));
    dest->n = src->n;
}

void printLS(LS_t *ls) {
     for (int i = 0; i < ls->n; ++i) {
        for (int j = 0; j < ls->n; ++j)
            printf("%lf ", ls->A[i][j]);

        printf("%lf\n", ls->b[i]);
    }
}

void residueLS(LS_t *ls) {
    for (int i = 0; i < ls->n; ++i) {
        ls->r[i] = 0.0;
        for (int j = 0; j < ls->n; ++j) {
            ls->r[i] += ls->A[i][j] * ls->x[j];
        }
    }

    for (int i = 0; i < ls->n; ++i)
        ls->r[i] -= ls->b[i];
}

void printResolution(LS_t *ls) {
    residueLS(ls);

    for (int i = 0; i < ls->n; ++i)
        printf("%.12f ", ls->x[i]);
    printf("\n");

    for (int i = 0; i < ls->n; ++i)
        printf("%.12f ", ls->r[i]);
    printf("\n");
}
