#ifndef EG_H
#define EG_H

void retrosSusbs(double **A, double *b, double *x, int n);

void eliminacaoGauss(double **A, double *b, int n);

void eliminacaoGaussTridiagonal(double *d, double *a, double *c, double *b, double *x, int n);

#endif // EG_H
