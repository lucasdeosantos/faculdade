#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <stdatomic.h>
#include "chrono.h"

#define MAX_THREADS 8
#define NTIMES 10

#define INPUT_SIZE 16000000
#define P_SIZE 1000

typedef struct {
    long long *Input;
    int n;
    long long *P;
    int np;
    long long *Output;
    unsigned int *Pos;
    int start;
    int end;
    atomic_int *range_index;
} ThreadData;

long long geraAleatorioLL() {
    int a = rand();
    int b = rand();
    return (long long)a * 100 + b;
}

int compare_LL(const void *a, const void *b) {
    long long diff = *(long long *)a - *(long long *)b;
    if (diff < 0) {
        return -1;
    }
    else if (diff > 0) {
        return 1;
    }
    else {
        return 0;
    }
}

int binary_search(long long *P, int np, long long value) {
    int start = 0, end = np - 1, mid;

    while (start < end) {
        mid = (start + end) / 2;
        if (value < P[mid]) {
            end = mid;
        } 
        else {
            start = mid + 1;
        }
    }
    return start;
}

void* multi_partition_thread(void* args) {
    ThreadData *data = (ThreadData*)args;

    for (int i = data->start; i < data->end; i++) {
        int range = binary_search(data->P, data->np, data->Input[i]);
        int index = atomic_fetch_add(&data->range_index[range], 1);
        data->Output[index] = data->Input[i];
    }

    pthread_exit(NULL);
}

void multi_partition(long long *Input, int n, long long *P, int np, long long *Output, unsigned int *Pos, int nThreads) {
    pthread_t threads[nThreads];
    ThreadData thread_data[nThreads];

    int range_count[np];
    for (int i = 0; i < np; i++) {
        range_count[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int range = binary_search(P, np, Input[i]);
        range_count[range]++;
    }

    Pos[0] = 0;
    for (int i = 1; i < np; i++) {
        Pos[i] = Pos[i - 1] + range_count[i - 1];
    }

    atomic_int range_index[np];
    for (int i = 0; i < np; i++) {
        atomic_init(&range_index[i], Pos[i]);
    }

    int range_per_thread = n / nThreads;

    for (int i = 0; i < nThreads; i++) {
        int start = i * range_per_thread;
        int end = (i == nThreads - 1) ? n : (i + 1) * range_per_thread;

        thread_data[i] = (ThreadData){Input, n, P, np, Output, Pos, start, end, range_index};
        pthread_create(&threads[i], NULL, multi_partition_thread, &thread_data[i]);
    }

    for (int t = 0; t < nThreads; t++) {
        pthread_join(threads[t], NULL);
    }
}

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
    unsigned int *Pos = (unsigned int *)malloc(np * sizeof(unsigned int));

    for (int i = 0; i < n; i++) {
        Input[i] = geraAleatorioLL();
    }

    for (int i = 0; i < np - 1; i++) {
        P[i] = geraAleatorioLL();
    }
    P[np - 1] = LLONG_MAX;
    qsort(P, np, sizeof(long long), (int (*)(const void *, const void *))compare_LL);

    chrono_reset(&multiPartitionTime);
    chrono_start(&multiPartitionTime);

    for(int i = 0; i < NTIMES; ++i)
        multi_partition(Input, n, P, np, Output, Pos, nThreads);

    chrono_stop(&multiPartitionTime);
    chrono_reportTime(&multiPartitionTime, "multiPartitionTime");

    verifica_particoes(Input, n, P, np, Output, Pos);

    double total_time_in_seconds = (double)chrono_gettotal(&multiPartitionTime) / ((double)1000*1000*1000);
    printf("total_time_in_seconds: %lf s\n", total_time_in_seconds);

    double EPS = ((double)INPUT_SIZE * NTIMES) / total_time_in_seconds;
    printf("Throughput: %lf EP/s\n", EPS);

    free(Input);
    free(P);
    free(Output);
    free(Pos);

    return 0;
}
