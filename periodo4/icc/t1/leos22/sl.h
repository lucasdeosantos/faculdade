#ifndef SL_H
#define SL_H

double **alocarMatriz(unsigned int n);

double *alocarVetor(unsigned int n);

void desalocarMatriz(double **m, unsigned int n);

void desalocarVetor(double *v);

void lerSL(double **A, double *b, unsigned int n);

void imprimirVetor(double *v, unsigned int n);

#endif // SL_H
