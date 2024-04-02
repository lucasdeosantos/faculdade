/*
* Author:
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#ifndef GS_H
#define GS_H

#include "ls.h"
#include "utils.h"

#define MAXIT 50

// Performs the Gauss-Seidel iterative method to solve a linear system, with a
// specified tolerance for convergence. Returns the number of iterations performed.
int gaussSeidel(LS_t *ls, real_t tol);

// Performs the Gauss-Seidel iterative method optimized for tridiagonal linear systems, with a
// specified tolerance for convergence. Returns the number of iterations performed.
int gaussSeidelTridiagonal(LS_t *ls, real_t tol);

#endif // GS_H
