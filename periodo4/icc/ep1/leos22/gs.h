#ifndef GS_H
#define GS_H

#define MAXIT 50

int gaussSeidel(double **A, double *b, double *x, int n, double tol);

int gaussSeidelTridiagonal(double *d, double *a, double *c, double *b, double *x, int n, double tol);

#endif // GS_H
