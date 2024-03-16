#include <stdio.h>
#include "eg.h"
#include "gs.h"
#include "sl.h"

int main () {
    unsigned int n;
    scanf("%d", &n);

    double **A = alocarMatriz(n);
    double *b = alocarVetor(n);
    double *x = alocarVetor(n);
    double *r = alocarVetor(n);

    lerSL(A, b, n);

    printf("EG clássico:\n");
    // resolver sistema linear com eliminacao de gauss
    // imprimir tempo em ms
    imprimirVetor(x, n);
    imprimirVetor(r, n);

    printf("GS clássico:\n");
    // resolver sistema linear com gauss-seigel
    // imprimir tempo em ms
    imprimirVetor(x, n);
    imprimirVetor(r, n);

    printf("EG 3-diagonal:\n");
    // resolver sistema 3-diagonal com eliminacao de gauss
    // imprimir tempo em ms
    imprimirVetor(x, n);
    imprimirVetor(r, n);

    printf("GS 3-diagonal:\n");
    // resolver sistema 3-diagonal com gauss-seigel
    // imprimir tempo em ms
    imprimirVetor(x, n);
    imprimirVetor(r, n);

    desalocarMatriz(A, n);
    desalocarVetor(b);
    desalocarVetor(x);
    desalocarVetor(r);
}
