#ifndef EG_H
#define EG_H

void retrosSusbs(double **A, double *b, double *x, unsigned int n);

void eliminacaoGauss(double **A, double *b, unsigned int n);

void eliminacaoGaussTridiagonal(double *d, double *a, double *c, double *b, double *x, unsigned int n);

#endif // EG_H
