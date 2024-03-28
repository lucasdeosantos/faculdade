#include <stdio.h>
#include <string.h>
#include "eg.h"
#include "gs.h"
#include "ls.h"
#include "utils.h"

int main () {
    unsigned int n;
    scanf("%d", &n);

    LS_t *ls = allocLS(n);
    LS_t *copy = allocLS(n);
    real_t *x = (real_t*) malloc(n * sizeof(real_t));
    real_t *r = (real_t*) malloc(n * sizeof(real_t));
    double tol = 1e-9;
    double time;
    int it;
    
    readLS(ls);

    copyLS(copy, ls);
    time = timestamp();
    // eliminacaoGauss(copy);
    // retrosSusbs(copy, x);
    time = timestamp() - time;
    residueLS(copy, x, r);
    printf("EG clássico:\n");
    printf("%.8lf ms\n", time);
    // imprimirVetor(x, n);
    // imprimirVetor(r, n);
    printf("\n");

    copyLS(copy, ls);
    time = timestamp();
    // it = gaussSeidel(copy, x, tol);
    time = timestamp() - time;
    residueLS(copy, x, r);
    printf("GS clássico [ %d iterações ]:\n", it);
    printf("%.8lf ms\n", time);
    // imprimirVetor(x, n);
    // imprimirVetor(r, n);
    printf("\n");

    real_t *d = (real_t*) malloc(n * sizeof(real_t));
    real_t *a = (real_t*) malloc(n * sizeof(real_t));
    real_t *c = (real_t*) malloc(n * sizeof(real_t));
    diagonalLS(ls, d, 0, 0);
    diagonalLS(ls, a, 1, 0);
    diagonalLS(ls, c, 0, 1);
    
    copyLS(copy, ls);
    memset(x, 0, n * sizeof(real_t));
    time = timestamp();
    // eliminacaoGaussTridiagonal(d, a, c, b, x, n);
    time = timestamp() - time;
    residueLS(copy, x, r);
    printf("EG 3-diagonal:\n");
    printf("%.8lf ms\n", time);
    // imprimirVetor(x, n);
    // imprimirVetor(r, n);
    printf("\n");

    copyLS(copy, ls);
    memset(x, 0, n * sizeof(real_t));
    time = timestamp();
    // it = gaussSeidelTridiagonal(d, a, c, b, x, n, tol);
    time = timestamp() - time;
    residueLS(copy, x, r);
    printf("GS 3-diagonal [ %d iterações ]:\n", it);
    printf("%.8lf ms\n", time);
    // imprimirVetor(x, n);
    // imprimirVetor(r, n);
    
    free(d);
    free(a);
    free(c);
    free(x);
    free(r);
}
