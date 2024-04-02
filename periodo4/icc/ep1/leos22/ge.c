/*
* Author:
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#include <string.h>
#include <stdlib.h>
#include "ge.h"

int findMax(LS_t *ls, int i) {
    real_t max = ls->A[i][i];
    int index = i;

    for (int j = i + 1; j < ls->n; ++j)
        if (ls->A[j][i] > max) {
            max = ls->A[j][i];
            index = j;
        }

    return index; 
}

void changeLine(LS_t *ls, unsigned int i, int iPivo) {
    real_t *temp = (real_t*) malloc(ls->n * sizeof(real_t));
    
    memcpy(temp, ls->A[i], ls->n * sizeof(real_t));

    for (int j = 0; j < ls->n; ++j) {
        ls->A[i][j] = ls->A[iPivo][j];
        ls->A[iPivo][j] = temp[j];
    }

    real_t aux = ls->b[i];
    ls->b[i] = ls->b[iPivo];
    ls->b[iPivo] = aux;

    free(temp);
}

void retrosSusbs(LS_t *ls) {
    for (int i = ls->n - 1; i >= 0; --i) {
        ls->x[i] = ls->b[i];
        for (int j = i + 1; j < ls->n; ++j)
            ls->x[i] -= ls->A[i][j] * ls->x[j];

        ls->x[i] /= ls->A[i][i];
    }
}

void gaussElimination(LS_t *ls) {
    for (int i = 0; i < ls->n; ++i) {
        int iPivo = findMax(ls, i);
        if (i != iPivo)
           changeLine(ls, i, iPivo);

        for (int k = i + 1; k < ls->n; ++k) {
            real_t m = ls->A[k][i] / ls->A[i][i];
            ls->A[k][i] = 0.0;
            for (int j = i + 1; j < ls->n; ++j)
                ls->A[k][j] -= ls->A[i][j] * m;
            ls->b[k] -= ls->b[i] * m;
        }
    }
    retrosSusbs(ls);
}

void gaussTridiagonalElimination(LS_t *ls) {
    real_t *d =(real_t*) malloc(ls->n * sizeof(real_t));
    real_t *a =(real_t*) malloc(ls->n * sizeof(real_t));
    real_t *c =(real_t*) malloc(ls->n * sizeof(real_t));

    diagonalLS(ls, d, 0, 0);
    diagonalLS(ls, a, 1, 0);
    diagonalLS(ls, c, 0, 1);

    for (int i = 0; i < ls->n - 1; ++i) {
        real_t m = a[i] / d[i];
        a[i] = 0.0;
        d[i + 1] -= c[i] * m;
        ls->b[i + 1] -= ls->b[i] * m;
    }
    
    ls->x[ls->n - 1] = ls->b[ls->n - 1] / d[ls->n - 1];
    for (int i = ls->n - 2; i >= 0; --i)
        ls->x[i] = (ls->b[i] - c[i] * ls->x[i + 1]) / d[i];

    free(d);
    free(a);
    free(c);
}
