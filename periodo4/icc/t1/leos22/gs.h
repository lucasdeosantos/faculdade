#ifndef GS_H
#define GS_H

#define MAXIT 50

void gaussSeidel(double **A, double *b, double *x, unsigned int n, double tol);

void gaussSeidelTridiagonais(double *d, double *a, double *c, double *b, double *x, unsigned int n, double tol);

#endif // GS_H
