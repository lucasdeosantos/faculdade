//
// By Lucas Emanuel de Oliveira Santos
//
#include <stdio.h>
#include <mpi.h>

void verifica_particoes(long long *Input, int n, long long *P, int np, long long *Output, int *nO) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int ok = 0;

    for (int i = 0; i < *nO; i++) {
        int found = 0;
        for (int j = 0; j < np; j++) {
            if (Output[i] >= P[j] && (j == np - 1 || Output[i] < P[j + 1])) {
                found = 1;
                break;
            }
        }
        if (!found) {
            ok = 1;
            break;
        }
    }

    if (ok)
        printf("Rank %d: ===> particionamento CORRETO\n", rank);
    else
        printf("Rank %d: ===> particionamento COM ERROS\n", rank);
}
