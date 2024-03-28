#ifndef LS_H
#define LS_H

typedef double real_t;

struct LS {
    real_t **A;
    real_t *b;
    real_t *x;
    real_t *r;
    int n;
};
typedef struct LS LS_t;

LS_t *allocLS(int n);

void freeLS(LS_t *ls);

void readLS(LS_t *ls);

void diagonalLS(LS_t *ls, real_t *d, int i, int j);

void copyLS(LS_t *dest, LS_t *src);

void printLS(LS_t *ls);

void printResolution(LS_t *ls);

#endif // LS_H
