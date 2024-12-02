//
// By Lucas Emanuel de Oliveira Santos
//
#include <stdio.h>

void verifica_particoes(long long *Input, int n, long long *P, int np, long long *Output, unsigned int *Pos) {
    for (int i = 0; i < np; i++) {
        int start = Pos[i];
        int end = (i == np - 1) ? n : Pos[i + 1];

        for (int j = start; j < end; j++) {
            if ((i == 0 && Output[j] >= P[i]) ||
                (i > 0 && (Output[j] < P[i - 1] || Output[j] >= P[i]))) {
                printf("===> particionamento COM ERROS\n");
                return;
            }
        }
    }
    printf("===> particionamento CORRETO\n");
}
