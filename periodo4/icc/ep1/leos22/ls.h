/*
* Author:
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#ifndef __LS_H__
#define __LS_H__

#include "utils.h"

// Represents a linear system of equations.
struct LS {
    real_t **A; // Matrix of coefficients for the equations.
    real_t *b; // Constants array.
    uint_t n; // Size of the matrix and array.
};
typedef struct LS LS_t;

// Allocates memory for a linear system with n variables and returns it.
LS_t *allocLS(uint_t n);

// Frees the memory allocated for ls.
void freeLS(LS_t *ls);

// Reads the coefficients and the constants of a linear system from input and stores them in ls.
void readLS(LS_t *ls);

// Computes a diagonal of the coefficients matrix A and stores it in the array d.
// The diagonal starts from position A[i][j].
void diagonalLS(LS_t *ls, real_t *d, uint_t i, uint_t j);

// Copies the contents of the linear system src to dst.
void copyLS(LS_t *dst, LS_t *src);

// Prints the coefficients matrix and the constants array of ls.
void printLS(LS_t *ls);

// Prints the solution and the residue of ls.
void printSolution(LS_t *ls, real_t *x, real_t *r);

#endif // __LS_H__
