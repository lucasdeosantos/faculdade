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
    double *a = alocarVetor(n-1);
    double *c = alocarVetor(n-1);
    double *x = alocarVetor(n);
    double *r = alocarVetor(n);
    double tol = 1e-9;
    int it;
    
    lerSL(A, b, n);

    copiarMatriz(A, copyA, n);
    copiarVetor(b, copyB, n);

    eliminacaoGauss(copyA, copyB, n);
    retrosSusbs(copyA, copyB, x, n);
    residuoSL(copyA, copyB, x, r, n);
    // imprimir tempo em ms
    printf("EG clássico:\n");
    imprimirVetor(x, n);
    imprimirVetor(r, n);
    
    copiarMatriz(A, copyA, n);
    copiarVetor(b, copyB, n);

    it = gaussSeidel(copyA, copyB, x, n, tol);
    residuoSL(copyA, copyB, x, r, n);
    // imprimir tempo em ms
    printf("GS clássico [ %d iterações ]:\n", it);
    imprimirVetor(x, n);
    imprimirVetor(r, n);
    
    diagonalMatriz(A, d, 0, 0, n);
    diagonalMatriz(A, a, 1, 0, n);
    diagonalMatriz(A, c, 0, 1, n);
    
    copiarMatriz(A, copyA, n);
    copiarVetor(b, copyB, n);

    eliminacaoGaussTridiagonal(d, a, c, b, x, n);
    residuoSL(copyA, copyB, x, r, n);
    // imprimir tempo em ms
    printf("EG 3-diagonal:\n");
    imprimirVetor(x, n);
    imprimirVetor(r, n);
       
    copiarMatriz(A, copyA, n);
    copiarVetor(b, copyB, n);

    it = gaussSeidelTridiagonal(d, a, c, b, x, n, tol);
    residuoSL(copyA, copyB, x, r, n);
    // imprimir tempo em ms
    printf("GS 3-diagonal [ %d iterações ]:\n", it);
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
