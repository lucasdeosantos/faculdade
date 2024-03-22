#ifndef SL_H
#define SL_H

double **alocarMatriz(int n);

void desalocarMatriz(double **m, int n);

void copiarMatriz(double **src, double **dest, int n);

void diagonalMatriz(double **m, double *d, int linha, int coluna, int n);

void imprimirMatriz(double **m, int n);

double *alocarVetor(int n);

void desalocarVetor(double *v);

void inicializarVetor(double *v, int n);

void copiarVetor(double *src, double *dest, int n);

void imprimirVetor(double *v, int n);

void lerSL(double **A, double *b, int n);

void imprimirSL(double **A, double *b, int n);

void residuoSL(double **A, double *b, double *x, double *r, int n);

#endif // SL_H
