#include <stdio.h>
#include "eg.h"
#include "gs.h"
#include "sl.h"
#include "utils.h"

int main () {
    unsigned int n;
    scanf("%d", &n);

    double **A = alocarMatriz(n);
    double **copyA = alocarMatriz(n);
    double *b = alocarVetor(n);
    double *copyB = alocarVetor(n);
    double *x = alocarVetor(n);
    double *r = alocarVetor(n);
    double tol = 1e-9;
    double tempo;
    int it;
    
    lerSL(A, b, n);

    copiarMatriz(A, copyA, n);
    copiarVetor(b, copyB, n);
    
    tempo = timestamp();
    eliminacaoGauss(copyA, copyB, n);
    retrosSusbs(copyA, copyB, x, n);
    tempo = timestamp() - tempo;
    residuoSL(copyA, copyB, x, r, n);
    printf("EG clássico:\n");
    printf("%.8lf ms\n", tempo);
    imprimirVetor(x, n);
    imprimirVetor(r, n);
    printf("\n");
    
    copiarMatriz(A, copyA, n);
    copiarVetor(b, copyB, n);
    inicializarVetor(x, n);

    tempo = timestamp();
    it = gaussSeidel(copyA, copyB, x, n, tol);
    tempo = timestamp() - tempo;
    residuoSL(copyA, copyB, x, r, n);
    printf("GS clássico [ %d iterações ]:\n", it);
    printf("%.8lf ms\n", tempo);
    imprimirVetor(x, n);
    imprimirVetor(r, n);
    printf("\n");

    double *d = alocarVetor(n);
    double *a = alocarVetor(n-1);
    double *c = alocarVetor(n-1);
    diagonalMatriz(A, d, 0, 0, n);
    diagonalMatriz(A, a, 1, 0, n);
    diagonalMatriz(A, c, 0, 1, n);
    
    copiarMatriz(A, copyA, n);
    copiarVetor(b, copyB, n);

    tempo = timestamp();
    eliminacaoGaussTridiagonal(d, a, c, b, x, n);
    tempo = timestamp() - tempo;
    residuoSL(copyA, copyB, x, r, n);
    printf("EG 3-diagonal:\n");
    printf("%.8lf ms\n", tempo);
    imprimirVetor(x, n);
    imprimirVetor(r, n);
    printf("\n");
       
    copiarMatriz(A, copyA, n);
    copiarVetor(b, copyB, n);
    inicializarVetor(x, n);

    tempo = timestamp();
    it = gaussSeidelTridiagonal(d, a, c, b, x, n, tol);
    tempo = timestamp() - tempo;
    residuoSL(copyA, copyB, x, r, n);
    printf("GS 3-diagonal [ %d iterações ]:\n", it);
    printf("%.8lf ms\n", tempo);
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
