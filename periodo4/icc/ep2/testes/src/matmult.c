#include <stdio.h>
#include <stdlib.h>    /* exit, malloc, calloc, etc. */
#include <string.h>
#include <getopt.h>    /* getopt */
#include <likwid.h>
#include "matriz.h"
#include "utils.h"


/**
 * Exibe mensagem de erro indicando forma de uso do programa e termina
 * o programa.
 */
static void usage(char *progname)
{
    fprintf(stderr, "Forma de uso: %s [ <ordem> ] \n", progname);
    exit(1);
}


/**
 * Programa principal
 * Forma de uso: matmult [ -n <ordem> ]
 * -n <ordem>: ordem da matriz quadrada e dos vetores
 *
 */
int main (int argc, char *argv[]) 
{
    int n=DEF_SIZE;

    MatRow mRow_1, mRow_2, resMat;
    Vetor vet, res;
    rtime_t time;

    /* =============== TRATAMENTO DE LINHA DE COMANDO =============== */

    if (argc < 2)
        usage(argv[0]);

    n = atoi(argv[1]);

    /* ================ FIM DO TRATAMENTO DE LINHA DE COMANDO ========= */

    srandom(20232);

    printf("%d,", n);

    res = geraVetor (n, 0); // (real_t *) malloc (n*sizeof(real_t));
    resMat = geraMatRow(n, n, 1);

    mRow_1 = geraMatRow (n, n, 0);
    mRow_2 = geraMatRow (n, n, 0);

    vet = geraVetor (n, 0);

    if (!res || !resMat || !mRow_1 || !mRow_2 || !vet) {
        fprintf(stderr, "Falha em alocação de memória !!\n");
        liberaVetor ((void*) mRow_1);
        liberaVetor ((void*) mRow_2);
        liberaVetor ((void*) resMat);
        liberaVetor ((void*) vet);
        liberaVetor ((void*) res);
        exit(2);
    }

#ifdef _DEBUG_
    prnMat (mRow_1, n, n);
    prnMat (mRow_2, n, n);
    prnVetor (vet, n);
    printf ("=================================\n\n");
#endif /* _DEBUG_ */

    LIKWID_MARKER_INIT;

    LIKWID_MARKER_START("MULT_MAT_VET");
    time = timestamp();
    multMatVet (mRow_1, vet, n, n, res);
    time = timestamp() - time; 
    printf("%.8lf,", time);
    LIKWID_MARKER_STOP("MULT_MAT_VET");
    
    LIKWID_MARKER_START("MULT_MAT_VET_UF_4");
    time = timestamp();
    multMatVetUnrollJam4 (mRow_1, vet, n, n, res);
    time = timestamp() - time; 
    printf("%.8lf,", time);
    LIKWID_MARKER_STOP("MULT_MAT_VET_UF_4");

    LIKWID_MARKER_START("MULT_MAT_VET_UF_8");
    time = timestamp();
    multMatVetUnrollJam8 (mRow_1, vet, n, n, res);
    time = timestamp() - time; 
    printf("%.8lf,", time);
    LIKWID_MARKER_STOP("MULT_MAT_VET_UF_8");

    LIKWID_MARKER_START("MULT_MAT_VET_UF_16");
    time = timestamp();
    multMatVetUnrollJam16 (mRow_1, vet, n, n, res);
    time = timestamp() - time; 
    printf("%.8lf,", time);
    LIKWID_MARKER_STOP("MULT_MAT_VET_UF_16");

    LIKWID_MARKER_START("MULT_MAT_VET_UF_32");
    time = timestamp();
    multMatVetUnrollJam32 (mRow_1, vet, n, n, res);
    time = timestamp() - time; 
    printf("%.8lf,", time);
    LIKWID_MARKER_STOP("MULT_MAT_VET_UF_32");

    LIKWID_MARKER_START("MULT_MAT_VET_UF_48");
    time = timestamp();
    multMatVetUnrollJam48 (mRow_1, vet, n, n, res);
    time = timestamp() - time; 
    printf("%.8lf,", time);
    LIKWID_MARKER_STOP("MULT_MAT_VET_UF_48");

    LIKWID_MARKER_START("MULT_MAT_VET_UF_64");
    time = timestamp();
    multMatVetUnrollJam64 (mRow_1, vet, n, n, res);
    time = timestamp() - time; 
    printf("%.8lf,", time);
    LIKWID_MARKER_STOP("MULT_MAT_VET_UF_64");


    LIKWID_MARKER_START("MULT_MAT_MAT");
    time = timestamp();
    multMatMat (mRow_1, mRow_2, n, resMat);
    time = timestamp() - time; 
    printf("%.8lf,", time);
    LIKWID_MARKER_STOP("MULT_MAT_MAT");

    int BK[] = {16, 32, 48, 64, 128, 256};
    for(int i = 0; i < 6; ++i) {
        memset(resMat, 0, n * n * sizeof(real_t));

        char markerStart[50];
        char markerStop[50];
        
        snprintf(markerStart, sizeof(markerStart), "MULT_MAT_MAT_BK_%d", BK[i]);
        snprintf(markerStop, sizeof(markerStop), "MULT_MAT_MAT_BK_%d", BK[i]);
        
        LIKWID_MARKER_START(markerStart);
        time = timestamp();
        multMatMatBlocking (mRow_1, mRow_2, n, resMat, BK[i]);
        time = timestamp() - time; 

        if (BK[i] == 256)
            printf("%.8lf\n", time);
        else
            printf("%.8lf,", time);

        LIKWID_MARKER_STOP(markerStop);
    }

    LIKWID_MARKER_CLOSE;

#ifdef _DEBUG_
    prnVetor (res, n);
    prnMat (resMat, n, n);
#endif /* _DEBUG_ */

    liberaVetor ((void*) mRow_1);
    liberaVetor ((void*) mRow_2);
    liberaVetor ((void*) resMat);
    liberaVetor ((void*) vet);
    liberaVetor ((void*) res);

    return 0;
}
