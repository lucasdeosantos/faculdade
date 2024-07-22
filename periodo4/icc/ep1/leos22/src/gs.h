/*
 * Author:
 * Lucas Emanuel de Oliveira Santos
 * GRR20224379
*/
#ifndef __GS_H__
#define __GS_H__

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

uint_t gaussSeidel(LS_t *ls, real_t *x, real_t tol);

// Method optimized for tridiagonal linear systems.
uint_t gaussSeidelTridiagonal(LS_t *ls, real_t *x, real_t tol);

#endif // __GS_H__
