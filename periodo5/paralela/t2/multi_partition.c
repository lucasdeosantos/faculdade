//
// By Lucas Emanuel de Oliveira Santos
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "chrono.h"
#include "verifica_particoes.h"

#define MAX_TOTAL_ELEMENTS (250*1000*1000)
#define MAX_THREADS 8
#define NTIMES 10
#define CALCULATE_RANGE_COUNT 1
#define CALCULATE_OUTPUT 2

typedef struct {
    int thread_id;
    long long *Input;
    int n;
    long long *P;
    int np;
    long long *Output;
    unsigned int *Pos;
    int *range_count;
    int *range_index;
    int *range_temp;
    int op;
} ThreadData;

pthread_t multiPartition_Threads[MAX_THREADS];
ThreadData multiPartition_thread_data[MAX_THREADS];
pthread_barrier_t multiPartition_Barrier;
int multiPartition_nThreads;

long long geraAleatorioLL() {
    int a = rand();
    int b = rand();
    return (long long)a * 100 + b;
}

int compare_LL(const void *a, const void *b) {
    long long diff = *(long long *)a - *(long long *)b;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
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

void *thread_worker(void *args) {
    ThreadData *data = (ThreadData *)args;

    int range_per_thread = data->n / multiPartition_nThreads;
    int start = data->thread_id * range_per_thread;
    int end = (data->thread_id == multiPartition_nThreads - 1) ? data->n : (data->thread_id + 1) * range_per_thread;

    while (1) {
        pthread_barrier_wait(&multiPartition_Barrier);

        if (data->op == CALCULATE_RANGE_COUNT) {
            int *local_range_count = (int *)calloc(data->np, sizeof(int));
            for (int i = start; i < end; i++) {
                int range = binary_search(data->P, data->np, data->Input[i]);
                local_range_count[range]++;
                data->range_temp[i] = range;
            }
            for (int i = 0; i < data->np; i++) {
                __sync_fetch_and_add(&data->range_count[i], local_range_count[i]);
            }
            free(local_range_count);
        } 
        /*
        else if (data->op == CALCULATE_OUTPUT) {
            for (int i = start; i < end; i++) {
                int range = data->range_temp[i];
                int index = __sync_fetch_and_add(&data->range_index[range], 1);
                data->Output[index] = data->Input[i];
            }
        }
        */
        else if (data->op == CALCULATE_OUTPUT) {
            int *local_range_index = (int *)calloc(data->np, sizeof(int));

            for (int i = start; i < end; i++) {
                int range = data->range_temp[i];
                local_range_index[range]++;
            }

            for (int range = 0; range < data->np; range++) {
                if (local_range_index[range] > 0) {
                    local_range_index[range] = __sync_fetch_and_add(&data->range_index[range], local_range_index[range]);
                }
            }

            for (int i = start; i < end; i++) {
                int range = data->range_temp[i];
                int index = local_range_index[range]++;
                data->Output[index] = data->Input[i];
            }

            free(local_range_index);
        }

        pthread_barrier_wait(&multiPartition_Barrier);
        if (data->thread_id == 0) {
            return NULL;
        }
    }
    if (data->thread_id != 0) {
        pthread_exit(NULL);
    }
    return NULL;
}

void multi_partition(long long *Input, int n, long long *P, int np, long long *Output, unsigned int *Pos) {
    static int initialized = 0;

    int *range_count = (int *)calloc(np, sizeof(int));
    int *range_index = (int *)calloc(np, sizeof(int));
    int *range_temp = (int *)malloc(n * sizeof(int));

    if (!initialized) {
        pthread_barrier_init(&multiPartition_Barrier, NULL, multiPartition_nThreads);
        
        for (int i = 1; i < multiPartition_nThreads; i++) {
            multiPartition_thread_data[i] = (ThreadData){i, Input, n, P, np, Output, Pos, range_count, range_index, range_temp, CALCULATE_RANGE_COUNT};
            pthread_create(&multiPartition_Threads[i], NULL, thread_worker, &multiPartition_thread_data[i]);
        }

        initialized = 1;
    }
    else {
        for (int i = 1; i < multiPartition_nThreads; i++) {
            multiPartition_thread_data[i] = (ThreadData){i, Input, n, P, np, Output, Pos, range_count, range_index, range_temp, CALCULATE_RANGE_COUNT};
        }
    }
    
    multiPartition_thread_data[0] = (ThreadData){0, Input, n, P, np, Output, Pos, range_count, range_index, range_temp, CALCULATE_RANGE_COUNT};
    thread_worker(&multiPartition_thread_data[0]);

    Pos[0] = 0;
    for (int i = 1; i < np; i++) {
        Pos[i] = Pos[i - 1] + range_count[i - 1];
        range_index[i] = Pos[i];
    }

    for (int i = 0; i < multiPartition_nThreads; i++) {
        multiPartition_thread_data[i].op = CALCULATE_OUTPUT;
    }
    thread_worker(&multiPartition_thread_data[0]);

    free(range_count);
    free(range_index);
    free(range_temp);
}

int main(int argc, char *argv[]) {
    int n = 8000000, np;
    char exp;
    chronometer_t multiPartitionTime;

    if (argc != 3) {
        printf("usage: %s <nThreads> <exp>\n", argv[0]);
        return 0;
    }
    else {
        multiPartition_nThreads = atoi(argv[1]);
        if (multiPartition_nThreads == 0) {
            printf("<nThreads> can't be 0\n");
            return 0;
        }
        if (multiPartition_nThreads > MAX_THREADS) {
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

    srand(42); // the answer to life, the universe, and everything

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

    long long *InputG = (long long *)malloc(MAX_TOTAL_ELEMENTS * sizeof(long long));
    for (int i  = 0; i < MAX_TOTAL_ELEMENTS / n; i++) {
        memcpy(&InputG[i * n], Input, n * sizeof(long long));
    }

    long long *PG = (long long *)malloc(MAX_TOTAL_ELEMENTS * sizeof(long long));
    for (int i  = 0; i < MAX_TOTAL_ELEMENTS / np; i++) {
        memcpy(&PG[i * np], P, np * sizeof(long long));
    }

    long long *InputCopy, *PCopy;
    int position_Input = 0, position_P = 0;

    chrono_reset(&multiPartitionTime);
    chrono_start(&multiPartitionTime);

    for(int i = 0; i < NTIMES; i++) {
        InputCopy = &InputG[position_Input];
        PCopy = &PG[position_P];

        multi_partition(InputCopy, n, PCopy, np, Output, Pos);

        position_Input += n; position_P += np;
        if (position_Input + n > MAX_TOTAL_ELEMENTS) position_Input = 0;
        if (position_P + np > MAX_TOTAL_ELEMENTS) position_P = 0;
    }

    chrono_stop(&multiPartitionTime);

    verifica_particoes(Input, n, P, np, Output, Pos);

    double total_time_in_seconds = (double)chrono_gettotal(&multiPartitionTime) / ((double)1000*1000*1000);
    printf("total_time_in_seconds: %lf s\n", total_time_in_seconds);

    double OPS = ((double)n * NTIMES) / total_time_in_seconds;
    printf("Throughput: %lf OP/s\n", OPS);

    free(Input);
    free(InputG);
    free(P);
    free(PG);
    free(Output);
    free(Pos);
    
    return 0;
}
