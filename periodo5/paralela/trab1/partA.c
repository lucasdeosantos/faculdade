// 
// By Lucas Emanuel de Oliveira Santos
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "chrono.h"

#define MAX_THREADS 8
#define MAX_TOTAL_ELEMENTS 16000000
#define QUERY_SIZE 100000

typedef struct {
    long long *inputVec;
    long long value;
    int nTotalElements;
    int nThreads;
    int thread_id;
} ThreadArgs;

pthread_t parallelReduce_Thread[MAX_THREADS];
ThreadArgs threads[MAX_THREADS];
int positions[MAX_THREADS];
pthread_barrier_t parallelReduce_barrier;
long long global_bsearch_operations = 0;
pthread_mutex_t ops_mutex;

int bsearch_lower_bound(long long *arr, int start, int end, long long value) {
    int left = start, right = end;
    while (left < right) {
        int mid = left + (right - left) / 2;
        pthread_mutex_lock(&ops_mutex);
        global_bsearch_operations++;
        pthread_mutex_unlock(&ops_mutex);
        if (arr[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

void *parallel_bsearch_worker(void *ptr) {
    ThreadArgs *args = (ThreadArgs *)ptr;
    int start = (args->nTotalElements / args->nThreads) * args->thread_id;
    int end = (args->thread_id == args->nThreads - 1) ? args->nTotalElements : start + (args->nTotalElements / args->nThreads);

    int pos = bsearch_lower_bound(args->inputVec, start, end, args->value);

    // Armazena a posição encontrada pela thread no vetor de posições
    positions[args->thread_id] = pos;

    return NULL;
}

int parallel_bsearch_lower_bound(long long *inputVec, int nTotalElements, long long value, int nThreads) {
    pthread_barrier_init(&parallelReduce_barrier, NULL, nThreads);
    pthread_mutex_init(&ops_mutex, NULL);

    for (int i = 0; i < nThreads; i++) {
        threads[i] = (ThreadArgs){inputVec, value, nTotalElements, nThreads, i};
        pthread_create(&parallelReduce_Thread[i], NULL, parallel_bsearch_worker, (void *)&threads[i]);
    }

    for (int i = 0; i < nThreads; i++) {
        pthread_join(parallelReduce_Thread[i], NULL);
    }

    int final_pos = nTotalElements;

    for (int i = 0; i < nThreads; i++) {
        int end = (i == nThreads - 1) ? nTotalElements : (nTotalElements / nThreads) * (i + 1);

        if (positions[i] < end) {
            final_pos = positions[i];
            break;
        }
    }

    pthread_mutex_destroy(&ops_mutex);
    pthread_barrier_destroy(&parallelReduce_barrier);
    return final_pos;
}

int compare(const void *a, const void *b) {
    return (*(long long *)a - *(long long *)b);
}

int main(int argc, char *argv[]) {
    int nThreads, nTotalElements;
    long long *InputVec, *queries;
    int *Pos;
    chronometer_t parallelBsearchTime;

    if(argc != 3) {
         printf("usage: %s <nTotalElements> <nThreads>\n", argv[0]);
         return 0;
    }
    else {
         nThreads = atoi(argv[2]);
         if(nThreads == 0) {
              printf("usage: %s <nTotalElements> <nThreads>\n", argv[0]);
              printf("<nThreads> can't be 0\n");
              return 0;
         }
         if(nThreads > MAX_THREADS) {
              printf("usage: %s <nTotalElements> <nThreads>\n", argv[0]);
              printf("<nThreads> must be less than %d\n", MAX_THREADS);
              return 0;
         }
         nTotalElements = atoi(argv[1]);
         if(nTotalElements > MAX_TOTAL_ELEMENTS) {
              printf("usage: %s <nTotalElements> <nThreads>\n", argv[0]);
              printf("<nTotalElements> must be up to %d\n", MAX_TOTAL_ELEMENTS);
              return 0;
         }
    }

    // Aloca memória para os vetores
    InputVec = (long long *)malloc(nTotalElements * sizeof(long long));
    queries = (long long *)malloc(QUERY_SIZE * sizeof(long long));
    Pos = (int *)malloc(QUERY_SIZE * sizeof(int));

    if (!InputVec || !queries || !Pos) {
        return 1;
    }

    srand(42);// The answer to life, the universe, and everything
    for (int i = 0; i < nTotalElements; i++) {
        InputVec[i] = rand() % (10 * nTotalElements);
    }
    qsort(InputVec, nTotalElements, sizeof(long long), (int (*)(const void *, const void *))compare);

    for (int i = 0; i < QUERY_SIZE; i++) {
        queries[i] = rand() % (10 * nTotalElements);
    }

    chrono_reset(&parallelBsearchTime);
    chrono_start(&parallelBsearchTime);

    for (int i = 0; i < QUERY_SIZE; i++)
        Pos[i] = parallel_bsearch_lower_bound(InputVec, nTotalElements, queries[i], nThreads);

    chrono_stop(&parallelBsearchTime);
    chrono_reportTime(&parallelBsearchTime, "parallelBsearchTime");

    double total_time_in_seconds = (double)chrono_gettotal(&parallelBsearchTime) / ((double)1000*1000*1000);
    printf("Total time: %lf seconds\n", total_time_in_seconds);

    double operations_per_second = ((double)global_bsearch_operations) / total_time_in_seconds;
    printf("Throughput: %lf OP/s\n", operations_per_second);

    free(InputVec);
    free(queries);
    free(Pos);

    return 0;
}