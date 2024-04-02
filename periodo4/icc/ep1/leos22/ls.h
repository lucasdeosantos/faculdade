/*
* Author:
*   Lucas Emanuel de Oliveira Santos
*   GRR20224379
*/
#ifndef LS_H
#define LS_H

#include "utils.h"

// Represents a linear system of equations.
struct LS {
    real_t **A; // Matrix of coefficients for the equations
    real_t *b;  // Constants array
    real_t *x;  // Variables array
    real_t *r;  // Residues array
    int n;      // Size of the matrix and arrays
};
typedef struct LS LS_t;

// Allocates memory for a linear system with "n" variables and returns it.
LS_t *allocLS(int n);

// Frees the memory allocated for the linear system "ls".
void freeLS(LS_t *ls);

// Reads the coefficients and the constants of a linear system from input and stores them in "ls".
void readLS(LS_t *ls);

// Computes the diagonal of the linear system "ls" and stores it in the array "d".
// The diagonal starts from position A[i][j].
void diagonalLS(LS_t *ls, real_t *d, int i, int j);

// Copies the contents of the linear system "src" to the linear system "dest".
void copyLS(LS_t *dest, LS_t *src);

// Prints the coefficients matrix and the constants vector of the linear system "ls".
void printLS(LS_t *ls);

// Prints the solution (variables array) and the residual array of the linear system "ls".
void printResolution(LS_t *ls);

#endif // LS_H
