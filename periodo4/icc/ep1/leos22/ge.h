#ifndef GE_H
#define GE_H

#include "ls.h"

void retrosSusbs(LS_t *ls, real_t *x);

void gaussElimination(LS_t *ls);

void gaussTridiagonalElimination(LS_t *ls, double *x);

#endif // GE_H
