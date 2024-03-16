#include <stdio.h>
#include <stdlib.h>

double **alocarMatriz(unsigned int n) {
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

double *alocarVetor(unsigned int n) {
    double *v = malloc(n * sizeof(double));
    if (!v) {
        fprintf(stderr, "erro ao alocar vetor.");
        exit(1);
    }

    return v;
}

void desalocarMatriz(double **m, unsigned int n) {
    for (int i = 0; i < n; ++i)
        free(m[i]);

    free(m);
}

void desalocarVetor(double *v) {
    free(v);
}

void lerSL(double **A, double *b, unsigned int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            scanf("%lf", &A[i][j]);

        scanf("%lf", &b[i]);
    }
}

void imprimirSL(double **A, double *b, unsigned int n) {
     for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            printf("%lf ", A[i][j]);

        printf("%lf\n", b[i]);
    }
}

void imprimirVetor(double *v, unsigned int n) {
    for (int i = 0; i < n; ++i)
        printf("%.12lf ", v[i]);
}
