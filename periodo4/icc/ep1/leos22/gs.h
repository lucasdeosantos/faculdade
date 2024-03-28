#ifndef GS_H
#define GS_H

#include "ls.h"

#define MAXIT 50

int gaussSeidel(LS_t *ls, double tol);

int gaussSeidelTridiagonal(LS_t *ls, double tol);

#endif // GS_H
