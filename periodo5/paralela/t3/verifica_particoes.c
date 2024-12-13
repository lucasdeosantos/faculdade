//
// By Lucas Emanuel de Oliveira Santos
//
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "binary_search.h"

void verifica_particoes(long long *Input, int n, long long *P, int np, long long *Output, int *nO) {
    int *aux_p = calloc(np, sizeof(int));
    int *pos = malloc(sizeof(int) * np);

    for (int i = 0; i < n; i++) {
        int aux_pos = binary_search(P, np, Input[i]);
        aux_p[aux_pos]++;
    }

    pos[0] = 0;
    for (int i = 1; i < np; i++) {
        pos[i] = pos[i - 1] + aux_p[i - 1];
    }

    for (int i = 0; i < np; i++) {
        int start = pos[i];
        int end = (i == np - 1) ? n : pos[i + 1];
        for (int j = start; j < end; j++) {
            if ((i == 0 && Output[j] >= P[i]) || (i > 0 && (Output[j] < P[i - 1] || Output[j] >= P[i]))) {
                printf("===> particionamento COM ERROS\n");
                return;
            }
        }
    }
    printf("===> particionamento CORRETO\n");

    free(aux_p);
    free(pos);
}
