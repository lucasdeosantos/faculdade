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

void calculate_pos(long long *Input, int n, long long *P, int np, unsigned int *Pos) {
    int counts[np];
    memset(counts, 0, sizeof(counts));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < np; j++) {
            if ((j == 0 && Input[i] < P[j]) ||
                (j > 0 && Input[i] >= P[j - 1] && Input[i] < P[j])) {
                counts[j]++;
                break;
            }
        }
    }

    Pos[0] = 0;
    for (int i = 1; i < np; i++) {
        Pos[i] = Pos[i - 1] + counts[i - 1];
    }
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

void multi_partition_pthreads(long long *Input, int n, long long *P, int np, long long *Output, unsigned int *Pos, int nThreads) {
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
/*
void *multi_partition_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    int local_counts[data->np];
    memset(local_counts, 0, sizeof(local_counts));

    for (int i = 0; i < data->n; i++) {
        for (int j = data->range_beg; j < data->range_end; j++) {
            if ((j == 0 && data->Input[i] < data->P[j]) ||
                (j > 0 && data->Input[i] >= data->P[j - 1] && data->Input[i] < data->P[j])) {
                local_counts[j]++;
                int write_index = data->Pos[j] + local_counts[j] - 1;
                data->Output[write_index] = data->Input[i];
                break;
            }
        }
    }

    for (int i = 0; i < data->np; i++) {
        data->counts[i] += local_counts[i];
    }

    return NULL;
}

void multi_partition(long long *Input, int n, long long *P, int np, long long *Output, unsigned int *Pos, int nThreads) {
    pthread_t threads[nThreads];
    ThreadData thread_data[nThreads];
    int counts[np];
    memset(counts, 0, sizeof(counts));

    int range_per_thread = np / nThreads;
    int extra_ranges = np % nThreads;

    for (int i = 0; i < nThreads; i++) {
        int range_beg = i * range_per_thread + (i < extra_ranges ? i : extra_ranges);
        int range_end = range_beg + range_per_thread + (i < extra_ranges ? 1 : 0);

        thread_data[i] = (ThreadData){Input, n, P, np, Output, Pos, range_beg, range_end, counts};
        pthread_create(&threads[i], NULL, multi_partition_thread, &thread_data[i]);
    }

    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    Pos[0] = 0;
    for (int i = 1; i < np; i++) {
        Pos[i] = Pos[i - 1] + counts[i - 1];
    }
}

void *multi_partition_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    int local_counts[data->np];
    memset(local_counts, 0, sizeof(local_counts));

    for (int i = 0; i < data->n; i++) {
        for (int j = data->range_beg; j < data->range_end; j++) {
            if ((j == 0 && data->Input[i] < data->P[j]) ||
                (j > 0 && data->Input[i] >= data->P[j - 1] && data->Input[i] < data->P[j])) {
                local_counts[j]++;
                
                // Usar operações atômicas para garantir a segurança do acesso
                int write_index = atomic_fetch_add(&data->write_index[j], 1);  // Incrementa de forma atômica
                data->Output[write_index] = data->Input[i];
                break;
            }
        }
    }

    // Atualiza o contador global após as operações da thread
    for (int i = 0; i < data->np; i++) {
        data->counts[i] += local_counts[i];
    }

    return NULL;
}

void multi_partition(long long *Input, int n, long long *P, int np, long long *Output, unsigned int *Pos, int nThreads) {
    pthread_t threads[nThreads];
    ThreadData thread_data[nThreads];
    int counts[np];
    memset(counts, 0, sizeof(counts));

    atomic_int write_index[np];  // Array de variáveis atômicas para controlar os índices de escrita
    for (int i = 0; i < np; i++) {
        atomic_init(&write_index[i], Pos[i]);  // Inicializa as posições atômicas
    }

    // Divide as faixas de trabalho para cada thread
    int range_per_thread = np / nThreads;
    int extra_ranges = np % nThreads;

    // Criar as threads com as faixas de dados a serem processadas
    for (int i = 0; i < nThreads; i++) {
        int range_beg = i * range_per_thread + (i < extra_ranges ? i : extra_ranges);
        int range_end = range_beg + range_per_thread + (i < extra_ranges ? 1 : 0);

        thread_data[i] = (ThreadData){Input, n, P, np, Output, Pos, write_index, range_beg, range_end, counts};
        pthread_create(&threads[i], NULL, multi_partition_thread, &thread_data[i]);
    }

    // Aguardar o término das threads
    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Calcular as posições finais para cada partição
    Pos[0] = 0;
    for (int i = 1; i < np; i++) {
        Pos[i] = Pos[i - 1] + counts[i - 1];
    }
}

void *multi_partition_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    for (int range = data->range_beg; range < data->range_end; range++) {
        int write_index = data->Pos[range];
        for (int i = 0; i < data->n; i++) {
            if ((range == 0 && data->Input[i] < data->P[range]) ||
                (range > 0 && data->Input[i] >= data->P[range - 1] && data->Input[i] < data->P[range])) {
                data->Output[write_index++] = data->Input[i];
            }
        }
    }
    return NULL;
}

void multi_partition_pthreads(long long *Input, int n, long long *P, int np, long long *Output, unsigned int *Pos, int nThreads) {
    pthread_t threads[nThreads];
    ThreadData thread_data[nThreads];

    calculate_pos(Input, n, P, np, Pos);

    int range_per_thread = np / nThreads;
    int extra_ranges = np % nThreads;

    for (int i = 0; i < nThreads; i++) {
        int range_beg = i * range_per_thread + (i < extra_ranges ? i : extra_ranges);
        int range_end = range_beg + range_per_thread + (i < extra_ranges ? 1 : 0);

        thread_data[i] = (ThreadData){Input, n, P, np, Output, Pos, range_beg, range_end, NULL};
        pthread_create(&threads[i], NULL, multi_partition_thread, &thread_data[i]);
    }

    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i], NULL);
    }
}
*/
void multi_partition(long long *Input, int n, long long *P, int np, long long *Output, unsigned int *Pos) {
    int currentIndex = 0;

    for (int faixa = 0; faixa < np; faixa++) {
        Pos[faixa] = currentIndex;
        for (int i = 0; i < n; i++) {
            if ((faixa == 0 && Input[i] < P[faixa]) ||
                (faixa > 0 && Input[i] >= P[faixa - 1] && Input[i] < P[faixa])) {
                Output[currentIndex++] = Input[i];
            }
        }
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

    printf("============================ Normal ============================\n");

    chrono_reset(&multiPartitionTime);
    chrono_start(&multiPartitionTime);

//    calculate_pos(Input, n, P, np, Pos);

//    for (int i = 0; i < NTIMES; ++i)
//        multi_partition(Input, n, P, np, Output, Pos, nThreads);
        multi_partition(Input, n, P, np, Output, Pos);

    chrono_stop(&multiPartitionTime);
    chrono_reportTime(&multiPartitionTime, "multiPartitionTime");
/*
    printf("Input: ");
    for (int i = 0; i < n; i++)
        printf("%lld ", Input[i]);
    printf("\n");

    printf("Output: ");
    for (int i = 0; i < n; i++)
        printf("%lld ", Output[i]);
    printf("\n");

    printf("Pos: ");
    for (int i = 0; i < np; i++)
        printf("%u ", Pos[i]);
    printf("\n");
*/
    verifica_particoes(Input, n, P, np, Output, Pos);

    for(int i = 0; i < np; i++)
        Pos[i] = 0;

    for(int i = 0; i < np; i++)
        Output[i] = 0;

    printf("========================== Com threads ==========================\n");

    chrono_reset(&multiPartitionTime);
    chrono_start(&multiPartitionTime);

    multi_partition_pthreads(Input, n, P, np, Output, Pos, nThreads);

    chrono_stop(&multiPartitionTime);
    chrono_reportTime(&multiPartitionTime, "multiPartitionTime");
/*
    printf("Input: ");
    for (int i = 0; i < n; i++)
        printf("%lld ", Input[i]);
    printf("\n");

    printf("Output: ");
    for (int i = 0; i < n; i++)
        printf("%lld ", Output[i]);
    printf("\n");

    printf("Pos: ");
    for (int i = 0; i < np; i++)
        printf("%u ", Pos[i]);
    printf("\n");
*/
    verifica_particoes(Input, n, P, np, Output, Pos);
/*
    calculate_pos(Input, n, P, np, Pos);

    printf("Pos: ");
    for (int i = 0; i < np; i++)
        printf("%u ", Pos[i]);
    printf("\n");
*/

//    double total_time_in_seconds = (double)chrono_gettotal(&multiPartitionTime) / ((double)1000*1000*1000);
//    printf("total_time_in_seconds: %lf s\n", total_time_in_seconds);

//    double OPS = ((double)TOTAL_ELEMENTS * NTIMES) / total_time_in_seconds;
//    printf("Throughput: %lf OP/s\n", OPS);

    free(Input);
    free(P);
    free(Output);
    free(Pos);

    return 0;
}
