/*
* Author:
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#ifndef GE_H
#define GE_H

#include "ls.h"

// Performs Gaussian elimination to solve a linear system.
void gaussElimination(LS_t *ls, real_t *x);

// Performs Gaussian elimination for tridiagonal linear systems.
void gaussTridiagonalElimination(LS_t *ls, real_t *x);

#endif // GE_H
