/*
* Author:
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#ifndef GS_H
#define GS_H

#include "ls.h"

// Max number of iterations.
#define MAXIT 50

/*
* The following methods perform the Gauss-Seidel iterative method to solve a linear system
* and return the number of iterations performed.
* ls is the linear system.
* x is the solution array.
* tol is a specified tolerance for convergence.
*/

int gaussSeidel(LS_t *ls, real_t *x, real_t tol);

// Method optimized for tridiagonal linear systems.
int gaussSeidelTridiagonal(LS_t *ls, real_t *x, real_t tol);

#endif // GS_H
