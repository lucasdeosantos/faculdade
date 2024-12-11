//
// By Lucas Emanuel de Oliveira Santos
//
#include <stdio.h>
#include <limits.h>
#include <mpi.h>

void verifica_particoes(long long *Input, int n, long long *P, int np, long long *Output, int *nO) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int ok = 1;

    for (int i = 0; i < *nO; ++i) {
        if (rank == 0) {
            if (Output[i] >= P[rank]) {
                ok = 0;
                break;
            }
        } 
        else if (rank == np - 1) {
            if (Output[i] < P[rank - 1]) {
                ok = 0;
                break;
            }
        } 
        else {
            if (Output[i] < P[rank - 1] || Output[i] >= P[rank]) {
                ok = 0;
                break;
            }
        }
    }

    if (ok) {
        printf("===> particionamento CORRETO no processo %d\n", rank);
    } else {
        printf("===> particionamento COM ERROS no processo %d\n", rank);
    }
}
