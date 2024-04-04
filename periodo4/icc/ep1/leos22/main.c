/*
* Author: 
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#include <stdio.h>
#include <string.h>
#include <likwid.h>
#include "utils.h"
#include "ls.h"
#include "ge.h"
#include "gs.h"

int main ()
{
    uint_t n;
    scanf("%d", &n);

    LS_t *ls = allocLS(n);
    LS_t *copy = allocLS(n);
    real_t *x = (real_t *) malloc(n * sizeof(real_t));
    real_t *r = (real_t *) malloc(n * sizeof(real_t));
    real_t tol = 1e-9;
    rtime_t time;
    uint_t it;
    
    readLS(ls);

    LIKWID_MARKER_INIT;

    copyLS(copy, ls);
    memset(x, 0, n * sizeof(real_t));
    time = timestamp();
    LIKWID_MARKER_START("EG_CLASSICO");
    gaussElimination(copy, x);
    LIKWID_MARKER_STOP("EG_CLASSICO");
    time = timestamp() - time;
    printf("EG clássico:\n");
    printf("%.8lf ms\n", time);
    printSolution(copy, x, r);
    printf("\n");

    copyLS(copy, ls);
    memset(x, 0, n * sizeof(real_t));
    time = timestamp();
    LIKWID_MARKER_START("GS_CLASSICO");
    it = gaussSeidel(copy, x, tol);
    LIKWID_MARKER_STOP("GS_CLASSICO");
    time = timestamp() - time;
    printf("GS clássico [ %d iterações ]:\n", it);
    printf("%.8lf ms\n", time);
    printSolution(copy, x, r);
    printf("\n");

    copyLS(copy, ls);
    memset(x, 0, n * sizeof(real_t));
    time = timestamp();
    LIKWID_MARKER_START("EG_TRIDIAGONAL");
    gaussTridiagonalElimination(copy, x);
    LIKWID_MARKER_STOP("EG_TRIDIAGONAL");
    time = timestamp() - time;
    printf("EG 3-diagonal:\n");
    printf("%.8lf ms\n", time);
    printSolution(copy, x, r);
    printf("\n");

    copyLS(copy, ls);
    memset(x, 0, n * sizeof(real_t));
    time = timestamp();
    LIKWID_MARKER_START("GS_TRIDIAGONAL");
    it = gaussSeidelTridiagonal(copy, x, tol);
    LIKWID_MARKER_STOP("GS_TRIDIAGONAL");
    time = timestamp() - time;
    printf("GS 3-diagonal [ %d iterações ]:\n", it);
    printf("%.8lf ms\n", time);
    printSolution(copy, x, r);

    LIKWID_MARKER_CLOSE;

    freeLS(ls);
    freeLS(copy);
    free(x);
    free(r);
    
    return 0;
}
