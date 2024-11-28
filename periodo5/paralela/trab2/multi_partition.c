#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdatomic.h>
#include "chrono.h"
#include "verifica_particoes.h"

#define CACHE_SIZE 8000000 // Cache com aproximadamente 8Mib
#define INPUT_SIZE 8000000
#define MAX_THREADS 8
#define NTIMES 10

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

pthread_t multiPartition_Threads[MAX_THREADS];
ThreadData thread_data[MAX_THREADS];
pthread_barrier_t multiPartition_Barrier;

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

    pthread_barrier_wait(&multiPartition_Barrier);    
    pthread_exit(NULL);
}

void multi_partition(long long *Input, int n, long long *P, int np, long long *Output, unsigned int *Pos, int nThreads) {
    pthread_barrier_init(&multiPartition_Barrier, NULL, nThreads + 1);

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
        pthread_create(&multiPartition_Threads[i], NULL, multi_partition_thread, &thread_data[i]);
    }

    pthread_barrier_wait(&multiPartition_Barrier);
    pthread_barrier_destroy(&multiPartition_Barrier);
}

int main(int argc, char *argv[]) {
    int nThreads, np;
    char exp;
    chronometer_t multiPartitionTime;

    if (argc != 3) {
        printf("usage: %s <nThreads> <exp>\n", argv[0]);
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
        exp = toupper((unsigned char)argv[2][0]);
        if (exp == 'A' && strlen(argv[2]) == 1) {
            np = 1000;
        }
        else if (exp == 'B' && strlen(argv[2]) == 1) {
            np = 100000;
        }
        else {
            printf("<exp> must be A or B\n");
            return 0;
        }
    }

    srand(42); // The answer to life, the universe, and everything

    int n = INPUT_SIZE;

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

    long long *InputVec = (long long *)malloc(CACHE_SIZE * sizeof(long long));
    for (int i = 0; i < CACHE_SIZE / n; i++)
        memcpy(&InputVec[i * n], Input, n * sizeof(long long));

    long long *PVec = (long long *)malloc(CACHE_SIZE * sizeof(long long));
    for (int i = 0; i < CACHE_SIZE / np; i++)
        memcpy(&PVec[i * np], P, np * sizeof(long long));
    
    long long *InputCopy;
    long long *PCopy;
    int position_Input = 0;
    int position_P = 0;

    chrono_reset(&multiPartitionTime);
    chrono_start(&multiPartitionTime);

    for(int i = 0; i < NTIMES; i++) {
        InputCopy = &InputVec[position_Input];
        PCopy = &PVec[position_P];

        multi_partition(InputCopy, n, PCopy, np, Output, Pos, nThreads);

        position_Input += n; 
        position_P += np;
        if (position_Input + n > CACHE_SIZE) position_Input = 0;
        if (position_P + np > CACHE_SIZE) position_P = 0;
    }

    chrono_stop(&multiPartitionTime);

    verifica_particoes(Input, n, P, np, Output, Pos);

    double total_time_in_seconds = (double)chrono_gettotal(&multiPartitionTime) / ((double)1000*1000*1000);
    printf("total_time_in_seconds: %lf s\n", total_time_in_seconds);

    free(Input);
    free(P);
    free(Output);
    free(Pos);

    return 0;
}
