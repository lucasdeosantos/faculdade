/*
* Author:
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#ifndef __GE_H__
#define __GE_H__

#include "ls.h"

/*
* The following methods perform Gaussian elimination to solve a linear system.
* ls is the linear system.
* x is the solution array.
*/

void gaussElimination(LS_t *ls, real_t *x);

// Method optimized for tridiagonal linear systems.
void gaussTridiagonalElimination(LS_t *ls, real_t *x);

#endif // __GE_H__
