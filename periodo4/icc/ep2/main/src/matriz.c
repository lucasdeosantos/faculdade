#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para uso de função 'memset()'
#include "matriz.h"
#include "utils.h"

/**
 * Função que gera valores para para ser usado em uma matriz
 * @param i,j coordenadas do elemento a ser calculado (0<=i,j<n)
 *  @return valor gerado para a posição i,j
 */
static inline real_t generateRandomA( unsigned int i, unsigned int j)
{
    static real_t invRandMax = 1.0 / (real_t)RAND_MAX;
    return ( (i == j) ? (real_t)(BASE<<1) : 1.0 )  * (real_t)random() * invRandMax;
}


/**
 * Função que gera valores aleatórios para ser usado em um vetor
 * @return valor gerado
 *
 */
static inline real_t generateRandomB( )
{
    static real_t invRandMax = 1.0 / (real_t)RAND_MAX;
    return (real_t)(BASE<<2) * (real_t)random() * invRandMax;
}


/* ----------- FUNÇÕES ---------------- */

/**
 *  Funcao geraMatRow: gera matriz como vetor único, 'row-oriented'
 * 
 *  @param m     número de linhas da matriz
 *  @param n     número de colunas da matriz
 *  @param zerar se 0, matriz  tem valores aleatórios, caso contrário,
 *               matriz tem valores todos nulos
 *  @return  ponteiro para a matriz gerada
 *
 */
MatRow geraMatRow (int m, int n, int zerar)
{
    MatRow matriz = (real_t *) malloc(m*n*sizeof(real_t));

    if (matriz) {
        if (zerar)
            memset(matriz,0,m*n*sizeof(real_t));
        else
            for (int i = 0; i < m; ++i)
                for (int j = 0; j < n; ++j)
                    matriz[i*n+j] = generateRandomA(i, j);
    }
    return (matriz);
}


/**
 *  Funcao geraVetor: gera vetor de tamanho 'n'
 * 
 *  @param n  número de elementos do vetor
 *  @param zerar se 0, vetor  tem valores aleatórios, caso contrário,
 *               vetor tem valores todos nulos
 *  @return  ponteiro para vetor gerado
 *
 */
Vetor geraVetor (int n, int zerar)
{
    Vetor vetor = (real_t *) malloc(n*sizeof(real_t));

    if (vetor) {
        if (zerar)
            memset(vetor,0,n*sizeof(real_t));
        else
            for (int i = 0; i < n; ++i)
                vetor[i] = generateRandomB();
    }
    return (vetor);
}


/**
 *  \brief: libera vetor
 * 
 *  @param  ponteiro para vetor
 *
 */
void liberaVetor (void *vet)
{
    free(vet);
}


/**
 *  Funcao multMatVet:  Efetua multiplicacao entre matriz 'mxn' por vetor
 *                       de 'n' elementos
 *  @param mat matriz 'mxn'
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *  @param res vetor que guarda o resultado. Deve estar previamente alocado e com
 *             seus elementos inicializados em 0.0 (zero)
 *  @return vetor de 'm' elementos
 *
 */
void multMatVet (MatRow mat, Vetor v, int m, int n, Vetor res)
{
    /* Efetua a multiplicação */
    if (res) {
        for (int i = 0; i < m; ++i) {
            res[i] = 0.0;
            for (int j = 0; j < n; ++j)
                res[i] += mat[n*i+j] * v[j];
        }
    }
}

void multMatVetUnrollJam (MatRow mat, Vetor v, int m, int n, Vetor res) {
    if (res) {
        for (int i = 0; i < m-m%UF; i += UF) {
            res[i] = res[i+1] = res[i+2] = res[i+3] = 0.0;
            res[i+4] = res[i+5] = res[i+6] = res[i+7] = 0.0;
            for(int j = 0; j < n; ++j) {
                res[i] += mat[n*i+j] * v[j];
                res[i+1] += mat[n*(i+1)+j] * v[j];
                res[i+2] += mat[n*(i+2)+j] * v[j];
                res[i+3] += mat[n*(i+3)+j] * v[j];
                res[i+4] += mat[n*(i+4)+j] * v[j];
                res[i+5] += mat[n*(i+5)+j] * v[j];
                res[i+6] += mat[n*(i+6)+j] * v[j];
                res[i+7] += mat[n*(i+7)+j] * v[j];
            }
        }

        for (int i = m-m%UF; i < m; ++i) {
            res[i] = 0.0;
            for(int j = 0; j < n; ++j)
                res[i] += mat[n*i+j] * v[j];
        }
    }
}


/**
 *  Funcao multMatMat: Efetua multiplicacao de duas matrizes 'n x n' 
 *  @param A matriz 'n x n'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *  @param C   matriz que guarda o resultado. Deve ser previamente gerada com 'geraMatPtr()'
 *             e com seus elementos inicializados em 0.0 (zero)
 *
 */
void multMatMat (MatRow A, MatRow B, int n, MatRow C)
{
    /* Efetua a multiplicação */
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            C[i*n+j] = 0.0;
            for (int k = 0; k < n; ++k)
                C[i*n+j] += A[i*n+k] * B[k*n+j];
        }
}

void multMatMatUnrollJam (MatRow A, MatRow B, int n, MatRow C) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n-n%UF; j += UF) {
            C[i*n+j] = C[i*n+j+1] = C[i*n+j+2] = C[i*n+j+3] = 0.0;
            C[i*n+j+4] = C[i*n+j+5] = C[i*n+j+6] = C[i*n+j+7] = 0.0;
            for (int k = 0; k < n; ++k) {
                C[i*n+j] += A[i*n+k] * B[k*n+j];
                C[i*n+j+1] += A[i*n+k] * B[k*n+j+1];
                C[i*n+j+2] += A[i*n+k] * B[k*n+j+2];
                C[i*n+j+3] += A[i*n+k] * B[k*n+j+3];
                C[i*n+j+4] += A[i*n+k] * B[k*n+j+4];
                C[i*n+j+5] += A[i*n+k] * B[k*n+j+5];
                C[i*n+j+6] += A[i*n+k] * B[k*n+j+6];
                C[i*n+j+7] += A[i*n+k] * B[k*n+j+7];
            }
        }
        for (int j = n-n%UF; j < n; ++j) {
            C[i*n+j] = 0.0;
            for (int k = 0; k < n; ++k)
                C[i*n+j] += A[i*n+k] * B[k*n+j];
        }
    }
}

void multMatMatBlocking (MatRow A, MatRow B, int n, MatRow C) {
    for (int i = 0; i < n; i += BK)
        for (int j = 0; j < n; j += BK)
            for (int k = 0; k < n; k += BK)
                for (int ii = i; ii < i + BK && ii < n; ++ii)
                    for (int jj = j; jj < j + BK && jj < n; ++jj)
                        for (int kk = k; kk < k + BK && kk < n; ++kk)
                            C[ii*n+jj] += A[ii*n+kk] * B[kk*n+jj];
}

void multMatMatUnrollJamBlocking (MatRow A, MatRow B, int n, MatRow C) {
    for (int i = 0; i < n; i += BK)
        for (int j = 0; j < n; j += BK)
            for (int k = 0; k < n; k += BK)
                for (int ii = i; ii < i + BK && ii < n; ++ii) {
                    for (int jj = j; jj < j + BK && jj < n-n%UF; jj += UF)
                        for (int kk = k; kk < k + BK && kk < n; ++kk) {
                            C[ii*n+jj] += A[ii*n+kk] * B[kk*n+jj];
                            C[ii*n+jj+1] += A[ii*n+kk] * B[kk*n+jj+1];
                            C[ii*n+jj+2] += A[ii*n+kk] * B[kk*n+jj+2];
                            C[ii*n+jj+3] += A[ii*n+kk] * B[kk*n+jj+3];
                            C[ii*n+jj+4] += A[ii*n+kk] * B[kk*n+jj+4];
                            C[ii*n+jj+5] += A[ii*n+kk] * B[kk*n+jj+5];
                            C[ii*n+jj+6] += A[ii*n+kk] * B[kk*n+jj+6];
                            C[ii*n+jj+7] += A[ii*n+kk] * B[kk*n+jj+7];
                        }

                    for (int jj = j + (n - n % UF); jj < j + BK && jj < n; ++jj)
                        for (int kk = k; kk < k + BK && kk < n; ++kk)
                            C[ii*n+jj] += A[ii*n+kk] * B[kk*n+jj];
                }
}


/**
 *  Funcao prnMat:  Imprime o conteudo de uma matriz em stdout
 *  @param mat matriz
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *
 */
void prnMat (MatRow mat, int m, int n)
{
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j)
            printf(DBL_FIELD, mat[n*i+j]);
        printf("\n");
    }
    printf(SEP_RES);
}


/**
 *  Funcao prnVetor:  Imprime o conteudo de vetor em stdout
 *  @param vet vetor com 'n' elementos
 *  @param n número de elementos do vetor
 *
 */
void prnVetor (Vetor vet, int n)
{
    for (int i = 0; i < n; ++i)
        printf(DBL_FIELD, vet[i]);
    printf(SEP_RES);
}
