#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <time.h>
#include "chrono.h"

#define MAX_THREADS 8
//#define MAX_TOTAL_ELEMENTS (250) * (1000) * (1000)
#define NTIMES 10

#define INPUT_SIZE 16000000
#define P_SIZE 1000

typedef struct {
    long long *input;
    int n;
    long long *p;
    int np;
    long long *output;
    long long *pos;
    int thread_id;
    int nThreads;
} Partition;

long long geraAleatorioLL() {
    int a = rand();
    int b = rand();
    return (long long)a * 100 + b;
}

int compare_LL(const void *a, const void *b) {
    return (*(long long *)a - *(long long *)b);
}

void *multi_partition_thread(void *arg) {
    Partition *args = (Partition *)arg;

    int chunk_size = args->n / args->nThreads;
    int start = args->thread_id * chunk_size;
    int end = (args->thread_id == args->nThreads - 1) ? args->n : start + chunk_size;

    for (int i = start; i < end; i++) {
        long long value = args->input[i];
        for (int j = 0; j < args->np; j++) {
            if (value <= args->p[j]) {
                args->output[i] = value;
                args->pos[i] = j;
                break;
            }
        }
    }

    return NULL;
}

void multi_partition(long long *input, int n, long long *p, int np, long long *output, long long *pos, int nThreads) {
    pthread_t threads[nThreads];
    Partition thread_args[nThreads];

    for (int i = 0; i < nThreads; i++) {
        thread_args[i] = (Partition){input, n, p, np, output, pos, i, nThreads};
        pthread_create(&threads[i], NULL, multi_partition_thread, &thread_args[i]);
    }

    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }
}

void verifica_particoes(long long *Input, int n, long long *P, int np, long long *Output, long long *Pos) {
    for (int i = 0; i < np - 1; i++) {
        for (int j = Pos[i]; j < Pos[i + 1]; j++) {
            if (!(Output[j] >= P[i] && Output[j] < P[i + 1])) {
                printf("===> particionamento COM ERROS\n");
                return;
            }
        }
    }
    printf("===> particionamento CORRETO\n");
}

int main(int argc, char *argv[]) {
    int nThreads;
    chronometer_t multiPartitionTime;

    if (argc != 2) {
        printf("usage: %s <nThreads>\n", argv[0]);
        return 0;
    } 
    else {
        nThreads = atoi(argv[1]);
        if (nThreads == 0) {
            printf("<nThreads> can't be 0\n");
            return 0;
        }
        if (nThreads > MAX_THREADS) {
            printf("<nThreads> must be less than or equal to %d\n", MAX_THREADS);
            return 0;
        }
    }

    srand(42); // The answer to life, the universe, and everything

    int n = INPUT_SIZE;
    int np = P_SIZE;
    
    long long *Input = (long long *)malloc(n * sizeof(long long));
    long long *P = (long long *)malloc(np * sizeof(long long));
    long long *Output = (long long *)malloc(n * sizeof(long long));
    long long *Pos = (long long *)malloc(np * sizeof(long long));

//    long long *InputCopy;
//    long long *PCopy;

    for (int i = 0; i < n; i++) {
        Input[i] = geraAleatorioLL();
    }

    for (int i = 0; i < np - 1; i++) {
        P[i] = geraAleatorioLL();
    }
    P[np - 1] = LLONG_MAX;
    qsort(P, np, sizeof(long long), (int (*)(const void *, const void *))compare_LL);

//    int start_position = 0;
//    Input = &Input[start_position];
//    P = &P[start_position];

    chrono_reset(&multiPartitionTime);
    chrono_start(&multiPartitionTime);

    for (int i = 0; i < NTIMES; ++i)
        multi_partition(Input, n, P, np, Output, Pos, nThreads);

    chrono_stop(&multiPartitionTime);
    chrono_reportTime(&multiPartitionTime, "multiPartitionTime");

//    printf("Input: ");
//    for (int i = 0; i < n; i++)
//        printf("%lld ", Input[i]);

    double total_time_in_seconds = (double)chrono_gettotal(&multiPartitionTime) / ((double)1000*1000*1000);
    printf("total_time_in_seconds: %lf s\n", total_time_in_seconds);

//    double OPS = ((double)TOTAL_ELEMENTS * NTIMES) / total_time_in_seconds;
//    printf("Throughput: %lf OP/s\n", OPS);

    verifica_particoes(Input, n, P, np, Output, Pos);

    free(Input);
    free(Output);
    free(Pos);
    free(P);

    return 0;
}
