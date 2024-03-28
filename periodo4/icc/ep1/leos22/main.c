#include <stdio.h>
#include <string.h>
#include "ge.h"
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
    gaussElimination(copy);
    retrosSusbs(copy, x);
    time = timestamp() - time;
    residueLS(copy, x, r);
    printf("EG clássico:\n");
    printf("%.8lf ms\n", time);
    // imprimirVetor(x, n);
    // imprimirVetor(r, n);
    printf("\n");

    copyLS(copy, ls);
    time = timestamp();
    it = gaussSeidel(copy, x, tol);
    time = timestamp() - time;
    residueLS(copy, x, r);
    printf("GS clássico [ %d iterações ]:\n", it);
    printf("%.8lf ms\n", time);
    // imprimirVetor(x, n);
    // imprimirVetor(r, n);
    printf("\n");
    
    copyLS(copy, ls);
    memset(x, 0, n * sizeof(real_t));
    time = timestamp();
    gaussTridiagonalElimination(copy, x);
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
    it = gaussSeidelTridiagonal(copy, x, tol);
    time = timestamp() - time;
    residueLS(copy, x, r);
    printf("GS 3-diagonal [ %d iterações ]:\n", it);
    printf("%.8lf ms\n", time);
    // imprimirVetor(x, n);
    // imprimirVetor(r, n);
    
    free(x);
    free(r);
}
