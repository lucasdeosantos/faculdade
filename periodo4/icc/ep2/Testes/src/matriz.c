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

void multMatVetUnrollJam4 (MatRow mat, Vetor v, int m, int n, Vetor res) {
    if (res) {
        for (int i = 0; i < m-m%4; i += 4) {
            res[i] = res[i+1] = res[i+2] = res[i+3] = 0.0;
            for(int j = 0; j < n; ++j) {
                res[i] += mat[n*i + j] * v[j];
                res[i+1] += mat[n*(i+1)+j] * v[j];
                res[i+2] += mat[n*(i+2)+j] * v[j];
                res[i+3] += mat[n*(i+3)+j] * v[j];
            }
        }

        for (int i = m-m%4; i < m; ++i) {
            res[i] = 0.0;
            for(int j = 0; j < n; ++j)
                res[i] += mat[n*i+j] * v[j];
        }
    }
}

void multMatVetUnrollJam8 (MatRow mat, Vetor v, int m, int n, Vetor res) {
    if (res) {
        for (int i = 0; i < m-m%8; i += 8) {
            res[i] = res[i+1] = res[i+2] = res[i+3] = 0.0;
            res[i+4] = res[i+5] = res[i+6] = res[i+7] = 0.0;
            for(int j = 0; j < n; ++j) {
                res[i] += mat[n*i + j] * v[j];
                res[i+1] += mat[n*(i+1)+j] * v[j];
                res[i+2] += mat[n*(i+2)+j] * v[j];
                res[i+3] += mat[n*(i+3)+j] * v[j];
                res[i+4] += mat[n*(i+4)+j] * v[j];
                res[i+5] += mat[n*(i+5)+j] * v[j];
                res[i+6] += mat[n*(i+6)+j] * v[j];
                res[i+7] += mat[n*(i+7)+j] * v[j];
            }
        }

        for (int i = m-m%8; i < m; ++i) {
            res[i] = 0.0;
            for(int j = 0; j < n; ++j)
                res[i] += mat[n*i+j] * v[j];
        }
    }
}

void multMatVetUnrollJam16 (MatRow mat, Vetor v, int m, int n, Vetor res) {
    if (res) {
        for (int i = 0; i < m-m%16; i += 16) {
            res[i] = res[i+1] = res[i+2] = res[i+3] = 0.0;
            res[i+4] = res[i+5] = res[i+6] = res[i+7] = 0.0;
            res[i+8] = res[i+9] = res[i+10] = res[i+11] = 0.0;
            res[i+12] = res[i+13] = res[i+14] = res[i+15] = 0.0;
            for(int j = 0; j < n; ++j) {
                res[i] += mat[n*i + j] * v[j];
                res[i+1] += mat[n*(i+1)+j] * v[j];
                res[i+2] += mat[n*(i+2)+j] * v[j];
                res[i+3] += mat[n*(i+3)+j] * v[j];
                res[i+4] += mat[n*(i+4)+j] * v[j];
                res[i+5] += mat[n*(i+5)+j] * v[j];
                res[i+6] += mat[n*(i+6)+j] * v[j];
                res[i+7] += mat[n*(i+7)+j] * v[j];
                res[i+8] += mat[n*(i+8)+j] * v[j];
                res[i+9] += mat[n*(i+9)+j] * v[j];
                res[i+10] += mat[n*(i+10)+j] * v[j];
                res[i+11] += mat[n*(i+11)+j] * v[j];
                res[i+12] += mat[n*(i+12)+j] * v[j];
                res[i+13] += mat[n*(i+13)+j] * v[j];
                res[i+14] += mat[n*(i+14)+j] * v[j];
                res[i+15] += mat[n*(i+15)+j] * v[j];
            }
        }

        for (int i = m-m%16; i < m; ++i) {
            res[i] = 0.0;
            for(int j = 0; j < n; ++j)
                res[i] += mat[n*i+j] * v[j];
        }
    }
}

void multMatVetUnrollJam32 (MatRow mat, Vetor v, int m, int n, Vetor res) {
    if (res) {
        for (int i = 0; i < m-m%32; i += 32) {
            res[i] = res[i+1] = res[i+2] = res[i+3] = 0.0;
            res[i+4] = res[i+5] = res[i+6] = res[i+7] = 0.0;
            res[i+8] = res[i+9] = res[i+10] = res[i+11] = 0.0;
            res[i+12] = res[i+13] = res[i+14] = res[i+15] = 0.0;
            res[i+16] = res[i+17] = res[i+18] = res[i+19] = 0.0;
            res[i+20] = res[i+21] = res[i+22] = res[i+23] = 0.0;
            res[i+24] = res[i+25] = res[i+26] = res[i+27] = 0.0;
            res[i+28] = res[i+29] = res[i+30] = res[i+31] = 0.0;
            for(int j = 0; j < n; ++j) {
                res[i] += mat[n*i + j] * v[j];
                res[i+1] += mat[n*(i+1)+j] * v[j];
                res[i+2] += mat[n*(i+2)+j] * v[j];
                res[i+3] += mat[n*(i+3)+j] * v[j];
                res[i+4] += mat[n*(i+4)+j] * v[j];
                res[i+5] += mat[n*(i+5)+j] * v[j];
                res[i+6] += mat[n*(i+6)+j] * v[j];
                res[i+7] += mat[n*(i+7)+j] * v[j];
                res[i+8] += mat[n*(i+8)+j] * v[j];
                res[i+9] += mat[n*(i+9)+j] * v[j];
                res[i+10] += mat[n*(i+10)+j] * v[j];
                res[i+11] += mat[n*(i+11)+j] * v[j];
                res[i+12] += mat[n*(i+12)+j] * v[j];
                res[i+13] += mat[n*(i+13)+j] * v[j];
                res[i+14] += mat[n*(i+14)+j] * v[j];
                res[i+15] += mat[n*(i+15)+j] * v[j];
                res[i+16] += mat[n*(i+16)+j] * v[j];
                res[i+17] += mat[n*(i+17)+j] * v[j];
                res[i+18] += mat[n*(i+18)+j] * v[j];
                res[i+19] += mat[n*(i+19)+j] * v[j];
                res[i+20] += mat[n*(i+20)+j] * v[j];
                res[i+21] += mat[n*(i+21)+j] * v[j];
                res[i+22] += mat[n*(i+22)+j] * v[j];
                res[i+23] += mat[n*(i+23)+j] * v[j];
                res[i+24] += mat[n*(i+24)+j] * v[j];
                res[i+25] += mat[n*(i+25)+j] * v[j];
                res[i+26] += mat[n*(i+26)+j] * v[j];
                res[i+27] += mat[n*(i+27)+j] * v[j];
                res[i+28] += mat[n*(i+28)+j] * v[j];
                res[i+29] += mat[n*(i+29)+j] * v[j];
                res[i+30] += mat[n*(i+30)+j] * v[j];
                res[i+31] += mat[n*(i+31)+j] * v[j];
            }
        }

        for (int i = m-m%32; i < m; ++i) {
            res[i] = 0.0;
            for(int j = 0; j < n; ++j)
                res[i] += mat[n*i+j] * v[j];
        }
    }
}

void multMatVetUnrollJam48 (MatRow mat, Vetor v, int m, int n, Vetor res) {
    if (res) {
        for (int i = 0; i < m-m%48; i += 48) {
            res[i] = res[i+1] = res[i+2] = res[i+3] = 0.0;
            res[i+4] = res[i+5] = res[i+6] = res[i+7] = 0.0;
            res[i+8] = res[i+9] = res[i+10] = res[i+11] = 0.0;
            res[i+12] = res[i+13] = res[i+14] = res[i+15] = 0.0;
            res[i+16] = res[i+17] = res[i+18] = res[i+19] = 0.0;
            res[i+20] = res[i+21] = res[i+22] = res[i+23] = 0.0;
            res[i+24] = res[i+25] = res[i+26] = res[i+27] = 0.0;
            res[i+28] = res[i+29] = res[i+30] = res[i+31] = 0.0;
            res[i+32] = res[i+33] = res[i+34] = res[i+35] = 0.0;
            res[i+36] = res[i+37] = res[i+38] = res[i+39] = 0.0;
            res[i+40] = res[i+41] = res[i+42] = res[i+43] = 0.0;
            res[i+44] = res[i+45] = res[i+46] = res[i+47] = 0.0;
            for(int j = 0; j < n; ++j) {
                res[i] += mat[n*i + j] * v[j];
                res[i+1] += mat[n*(i+1)+j] * v[j];
                res[i+2] += mat[n*(i+2)+j] * v[j];
                res[i+3] += mat[n*(i+3)+j] * v[j];
                res[i+4] += mat[n*(i+4)+j] * v[j];
                res[i+5] += mat[n*(i+5)+j] * v[j];
                res[i+6] += mat[n*(i+6)+j] * v[j];
                res[i+7] += mat[n*(i+7)+j] * v[j];
                res[i+8] += mat[n*(i+8)+j] * v[j];
                res[i+9] += mat[n*(i+9)+j] * v[j];
                res[i+10] += mat[n*(i+10)+j] * v[j];
                res[i+11] += mat[n*(i+11)+j] * v[j];
                res[i+12] += mat[n*(i+12)+j] * v[j];
                res[i+13] += mat[n*(i+13)+j] * v[j];
                res[i+14] += mat[n*(i+14)+j] * v[j];
                res[i+15] += mat[n*(i+15)+j] * v[j];
                res[i+16] += mat[n*(i+16)+j] * v[j];
                res[i+17] += mat[n*(i+17)+j] * v[j];
                res[i+18] += mat[n*(i+18)+j] * v[j];
                res[i+19] += mat[n*(i+19)+j] * v[j];
                res[i+20] += mat[n*(i+20)+j] * v[j];
                res[i+21] += mat[n*(i+21)+j] * v[j];
                res[i+22] += mat[n*(i+22)+j] * v[j];
                res[i+23] += mat[n*(i+23)+j] * v[j];
                res[i+24] += mat[n*(i+24)+j] * v[j];
                res[i+25] += mat[n*(i+25)+j] * v[j];
                res[i+26] += mat[n*(i+26)+j] * v[j];
                res[i+27] += mat[n*(i+27)+j] * v[j];
                res[i+28] += mat[n*(i+28)+j] * v[j];
                res[i+29] += mat[n*(i+29)+j] * v[j];
                res[i+30] += mat[n*(i+30)+j] * v[j];
                res[i+31] += mat[n*(i+31)+j] * v[j];
                res[i+32] += mat[n*(i+32)+j] * v[j];
                res[i+33] += mat[n*(i+33)+j] * v[j];
                res[i+34] += mat[n*(i+34)+j] * v[j];
                res[i+35] += mat[n*(i+35)+j] * v[j];
                res[i+36] += mat[n*(i+36)+j] * v[j];
                res[i+37] += mat[n*(i+37)+j] * v[j];
                res[i+38] += mat[n*(i+38)+j] * v[j];
                res[i+39] += mat[n*(i+39)+j] * v[j];
                res[i+40] += mat[n*(i+40)+j] * v[j];
                res[i+41] += mat[n*(i+41)+j] * v[j];
                res[i+42] += mat[n*(i+42)+j] * v[j];
                res[i+43] += mat[n*(i+43)+j] * v[j];
                res[i+44] += mat[n*(i+44)+j] * v[j];
                res[i+45] += mat[n*(i+45)+j] * v[j];
                res[i+46] += mat[n*(i+46)+j] * v[j];
                res[i+47] += mat[n*(i+47)+j] * v[j];
            }
        }

        for (int i = m-m%48; i < m; ++i) {
            res[i] = 0.0;
            for(int j = 0; j < n; ++j)
                res[i] += mat[n*i+j] * v[j];
        }
    }
}

void multMatVetUnrollJam64 (MatRow mat, Vetor v, int m, int n, Vetor res) {
    if (res) {
        for (int i = 0; i < m-m%64; i += 64) {
            res[i] = res[i+1] = res[i+2] = res[i+3] = 0.0;
            res[i+4] = res[i+5] = res[i+6] = res[i+7] = 0.0;
            res[i+8] = res[i+9] = res[i+10] = res[i+11] = 0.0;
            res[i+12] = res[i+13] = res[i+14] = res[i+15] = 0.0;
            res[i+16] = res[i+17] = res[i+18] = res[i+19] = 0.0;
            res[i+20] = res[i+21] = res[i+22] = res[i+23] = 0.0;
            res[i+24] = res[i+25] = res[i+26] = res[i+27] = 0.0;
            res[i+28] = res[i+29] = res[i+30] = res[i+31] = 0.0;
            res[i+32] = res[i+33] = res[i+34] = res[i+35] = 0.0;
            res[i+36] = res[i+37] = res[i+38] = res[i+39] = 0.0;
            res[i+40] = res[i+41] = res[i+42] = res[i+43] = 0.0;
            res[i+44] = res[i+45] = res[i+46] = res[i+47] = 0.0;
            res[i+48] = res[i+49] = res[i+50] = res[i+51] = 0.0;
            res[i+52] = res[i+53] = res[i+54] = res[i+55] = 0.0;
            res[i+56] = res[i+57] = res[i+58] = res[i+59] = 0.0;
            res[i+60] = res[i+61] = res[i+62] = res[i+63] = 0.0;
            for(int j = 0; j < n; ++j) {
                res[i] += mat[n*i + j] * v[j];
                res[i+1] += mat[n*(i+1)+j] * v[j];
                res[i+2] += mat[n*(i+2)+j] * v[j];
                res[i+3] += mat[n*(i+3)+j] * v[j];
                res[i+4] += mat[n*(i+4)+j] * v[j];
                res[i+5] += mat[n*(i+5)+j] * v[j];
                res[i+6] += mat[n*(i+6)+j] * v[j];
                res[i+7] += mat[n*(i+7)+j] * v[j];
                res[i+8] += mat[n*(i+8)+j] * v[j];
                res[i+9] += mat[n*(i+9)+j] * v[j];
                res[i+10] += mat[n*(i+10)+j] * v[j];
                res[i+11] += mat[n*(i+11)+j] * v[j];
                res[i+12] += mat[n*(i+12)+j] * v[j];
                res[i+13] += mat[n*(i+13)+j] * v[j];
                res[i+14] += mat[n*(i+14)+j] * v[j];
                res[i+15] += mat[n*(i+15)+j] * v[j];
                res[i+16] += mat[n*(i+16)+j] * v[j];
                res[i+17] += mat[n*(i+17)+j] * v[j];
                res[i+18] += mat[n*(i+18)+j] * v[j];
                res[i+19] += mat[n*(i+19)+j] * v[j];
                res[i+20] += mat[n*(i+20)+j] * v[j];
                res[i+21] += mat[n*(i+21)+j] * v[j];
                res[i+22] += mat[n*(i+22)+j] * v[j];
                res[i+23] += mat[n*(i+23)+j] * v[j];
                res[i+24] += mat[n*(i+24)+j] * v[j];
                res[i+25] += mat[n*(i+25)+j] * v[j];
                res[i+26] += mat[n*(i+26)+j] * v[j];
                res[i+27] += mat[n*(i+27)+j] * v[j];
                res[i+28] += mat[n*(i+28)+j] * v[j];
                res[i+29] += mat[n*(i+29)+j] * v[j];
                res[i+30] += mat[n*(i+30)+j] * v[j];
                res[i+31] += mat[n*(i+31)+j] * v[j];
                res[i+32] += mat[n*(i+32)+j] * v[j];
                res[i+33] += mat[n*(i+33)+j] * v[j];
                res[i+34] += mat[n*(i+34)+j] * v[j];
                res[i+35] += mat[n*(i+35)+j] * v[j];
                res[i+36] += mat[n*(i+36)+j] * v[j];
                res[i+37] += mat[n*(i+37)+j] * v[j];
                res[i+38] += mat[n*(i+38)+j] * v[j];
                res[i+39] += mat[n*(i+39)+j] * v[j];
                res[i+40] += mat[n*(i+40)+j] * v[j];
                res[i+41] += mat[n*(i+41)+j] * v[j];
                res[i+42] += mat[n*(i+42)+j] * v[j];
                res[i+43] += mat[n*(i+43)+j] * v[j];
                res[i+44] += mat[n*(i+44)+j] * v[j];
                res[i+45] += mat[n*(i+45)+j] * v[j];
                res[i+46] += mat[n*(i+46)+j] * v[j];
                res[i+47] += mat[n*(i+47)+j] * v[j];
                res[i+48] += mat[n*(i+48)+j] * v[j];
                res[i+49] += mat[n*(i+49)+j] * v[j];
                res[i+50] += mat[n*(i+50)+j] * v[j];
                res[i+51] += mat[n*(i+51)+j] * v[j];
                res[i+52] += mat[n*(i+52)+j] * v[j];
                res[i+53] += mat[n*(i+53)+j] * v[j];
                res[i+54] += mat[n*(i+54)+j] * v[j];
                res[i+55] += mat[n*(i+55)+j] * v[j];
                res[i+56] += mat[n*(i+56)+j] * v[j];
                res[i+57] += mat[n*(i+57)+j] * v[j];
                res[i+58] += mat[n*(i+58)+j] * v[j];
                res[i+59] += mat[n*(i+59)+j] * v[j];
                res[i+60] += mat[n*(i+60)+j] * v[j];
                res[i+61] += mat[n*(i+61)+j] * v[j];
                res[i+62] += mat[n*(i+62)+j] * v[j];
                res[i+63] += mat[n*(i+63)+j] * v[j];
            }
        }

        for (int i = m-m%64; i < m; ++i) {
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

void multMatMatBlocking (MatRow A, MatRow B, int n, MatRow C, int BK) {
    for (int i = 0; i < n; i += BK)
        for (int j = 0; j < n; j += BK)
            for (int k = 0; k < n; k += BK)
                for (int ii = i; ii < i + BK && ii < n; ++ii)
                    for (int jj = j; jj < j + BK && jj < n; ++jj)
                        for (int kk = k; kk < k + BK && kk < n; ++kk)
                            C[ii*n+jj] += A[ii*n+kk] * B[kk*n+jj];
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
