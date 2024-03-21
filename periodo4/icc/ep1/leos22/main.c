#include <stdio.h>
#include "eg.h"
#include "gs.h"
#include "sl.h"

int main () {
    unsigned int n;
    scanf("%d", &n);

    double **A = alocarMatriz(n);
    double **copyA = alocarMatriz(n);
    double *b = alocarVetor(n);
    double *copyB = alocarVetor(n);
    double *d = alocarVetor(n);
    double *a = alocarVetor(n);
    double *c = alocarVetor(n);
    double *x = alocarVetor(n);
    double *r = alocarVetor(n);
    double tol = 0.1;
    
    lerSL(A, b, n);

    copiarMatriz(A, copyA, n);
    copiarVetor(b, copyB, n);
    printf("EG clássico:\n");
    // resolver sistema linear com eliminacao de gauss
    eliminacaoGauss(copyA, copyB, n);
    retrosSusbs(copyA, copyB, x, n);
    residuoSL(copyA, copyB, x, r, n);
    // imprimir tempo em ms
    imprimirVetor(x, n);
    imprimirVetor(r, n);

    copiarMatriz(A, copyA, n);
    copiarVetor(b, copyB, n);
    printf("GS clássico [ 50 iterações ]:\n");
    // resolver sistema linear com gauss-seigel
    gaussSeidel(copyA, copyB, x, n, tol);
    residuoSL(copyA, copyB, x, r, n);
    // imprimir tempo em ms
    imprimirVetor(x, n);
    imprimirVetor(r, n);

    diagonalMatriz(A, d, 0, 0, n);
    diagonalMatriz(A, a, 1, 0, n);
    diagonalMatriz(A, c, 0, 1, n);

    copiarMatriz(A, copyA, n);
    copiarVetor(b, copyB, n);
    printf("EG 3-diagonal:\n");
    // resolver sistema 3-diagonal com eliminacao de gauss
    eliminacaoGaussTridiagonal(d, a, c, b, x, n);
    residuoSL(copyA, copyB, x, r, n);
    // imprimir tempo em ms
    imprimirVetor(x, n);
    imprimirVetor(r, n);

    copiarMatriz(A, copyA, n);
    copiarVetor(b, copyB, n);
    printf("GS 3-diagonal [ 50 iterações ]:\n");
    // resolver sistema 3-diagonal com gauss-seigel
    gaussSeidelTridiagonal(d, a, c, b, x, n, tol);
    residuoSL(copyA, copyB, x, r, n);
    // imprimir tempo em ms
    imprimirVetor(x, n);
    imprimirVetor(r, n);

    desalocarMatriz(A, n);
    desalocarMatriz(copyA, n);
    desalocarVetor(b);
    desalocarVetor(copyB);
    desalocarVetor(d);
    desalocarVetor(a);
    desalocarVetor(c);
    desalocarVetor(x);
    desalocarVetor(r);
}
