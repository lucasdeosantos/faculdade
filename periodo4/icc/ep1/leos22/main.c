/*
* Author: 
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#include <stdio.h>
#include "likwid.h"
#include "ge.h"
#include "gs.h"
#include "ls.h"
#include "utils.h"

int main () {
    LIKWID_MARKER_INIT;

    int n;
    scanf("%d", &n);

    LS_t *ls = allocLS(n);
    LS_t *copy = allocLS(n);
    real_t tol = 1e-9;
    rtime_t time;
    int it;
    
    readLS(ls);

    LIKWID_MARKER_START("EG_CLASSICO");
    copyLS(copy, ls);
    time = timestamp();
    gaussElimination(copy);
    time = timestamp() - time;
    printf("EG clássico:\n");
    printf("%.8lf ms\n", time);
    printResolution(copy);
    printf("\n");
    LIKWID_MARKER_STOP("EG_CLASSICO");

    LIKWID_MARKER_START("GS_CLASSICO");
    copyLS(copy, ls);
    time = timestamp();
    it = gaussSeidel(copy, tol);
    time = timestamp() - time;
    printf("GS clássico [ %d iterações ]:\n", it);
    printf("%.8lf ms\n", time);
    printResolution(copy);
    printf("\n");
    LIKWID_MARKER_STOP("GS_CLASSICO");

    LIKWID_MARKER_START("EG_TRIDIAGONAL");
    copyLS(copy, ls);
    time = timestamp();
    gaussTridiagonalElimination(copy);
    time = timestamp() - time;
    printf("EG 3-diagonal:\n");
    printf("%.8lf ms\n", time);
    printResolution(copy);
    printf("\n");
    LIKWID_MARKER_STOP("EG_TRIDIAGONAL");

    LIKWID_MARKER_START("GS_TRIDIAGONAL");
    copyLS(copy, ls);
    time = timestamp();
    it = gaussSeidelTridiagonal(copy, tol);
    time = timestamp() - time;
    printf("GS 3-diagonal [ %d iterações ]:\n", it);
    printf("%.8lf ms\n", time);
    printResolution(copy);
    LIKWID_MARKER_STOP("GS_TRIDIAGONAL");

    freeLS(ls);
    freeLS(copy);

    LIKWID_MARKER_CLOSE;
}
