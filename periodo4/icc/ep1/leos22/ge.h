/*
* Author:
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#ifndef GE_H
#define GE_H

#include "ls.h"

// Performs backward substitution to find the solution of a linear system.
void retrosSusbs(LS_t *ls);

// Performs Gaussian elimination to solve a linear system.
void gaussElimination(LS_t *ls);

// Performs Gaussian elimination for tridiagonal linear systems.
void gaussTridiagonalElimination(LS_t *ls);

#endif // GE_H
