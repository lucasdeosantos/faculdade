#include <stdio.h>
#include <stdlib.h>
#include "ls.h"

double **alocarMatriz(int n) {
    double **m = malloc(n * sizeof(double *));
    if (!m) {
        fprintf(stderr, "erro ao alocar matriz.");
        exit(1);
    }

    for (int i = 0; i < n; ++i) {
        m[i] = malloc(n * sizeof(double));
        if (!m[i]) {
            fprintf(stderr, "erro ao alocar matriz.");
            exit(1);
        }
    }

    return m;
}

void desalocarMatriz(double **m, int n) {
    for (int i = 0; i < n; ++i)
        free(m[i]);

    free(m);
}

void copiarMatriz(double **src, double **dest, int n) {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            dest[i][j] = src[i][j];
}

void diagonalMatriz(double **m, double *d, int linha, int coluna, int n) {
    for (int i = 0; i < n && linha + i < n && coluna + i < n; ++i)
        d[i] = m[linha + i][coluna + i];
}

void imprimirMatriz(double **m, int n) {
     for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            printf("%lf ", m[i][j]);
        printf("\n");
    }
}

double *alocarVetor(int n) {
    double *v = malloc(n * sizeof(double));
    if (!v) {
        fprintf(stderr, "erro ao alocar vetor.");
        exit(1);
    }

    return v;
}

void desalocarVetor(double *v) {
    free(v);
}

void inicializarVetor(double *v, int n) {
    for (int i = 0; i < n; ++i)
        v[i] = 0.0;
}

void copiarVetor(double *src, double *dest, int n) {
    for (int i = 0; i < n; ++i)
        dest[i] = src[i];
}

void imprimirVetor(double *v, int n) {
    for (int i = 0; i < n; ++i)
        printf("%.12lf ", v[i]);
    printf("\n");
}

void lerSL(double **A, double *b, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            scanf("%lf", &A[i][j]);

        scanf("%lf", &b[i]);
    }
}

void imprimirSL(double **A, double *b, int n) {
     for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            printf("%lf ", A[i][j]);

        printf("%lf\n", b[i]);
    }
}

void residuoSL(double **A, double *b, double *x, double *r, int n) {
    for (int i = 0; i < n; ++i) {
        r[i] = 0.0;
        for (int j = 0; j < n; ++j) {
            r[i] += A[i][j] * x[j];
        }
    }

    for (int i = 0; i < n; ++i)
        r[i] -= b[i];
}
