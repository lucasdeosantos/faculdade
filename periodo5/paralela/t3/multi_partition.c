//
// By Lucas Emanuel de Oliveira Santos
// 
#include <stdlib.h>
#include <mpi.h>
#include <limits.h>
#include "chrono.h"
#include "verifica_particoes.h"
#include "binary_search.h"

#define NTIMES 10

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

void multi_partition_mpi(long long *Input, int n, long long *P, int np, long long *Output, int *nO) {
    int *aux_p = calloc(np, sizeof(int));
    int *aux_pos = malloc(sizeof(int) * np);
    int *aux_N = calloc(np, sizeof(int));

    for (int i = 0; i < n; i++) {
        int aux_pos = binary_search(P, np, Input[i]);
        aux_p[aux_pos]++;
    }

    int pos = 0;
    for (int i = 0; i < np - 1; i++) {
        pos += aux_p[i];
        aux_N[i + 1] = pos;
    }

    for (int i = 0; i < np; i++) {
        aux_pos[i] = aux_N[i];
    }

    for (int i = 0; i < n; i++) {
        int part = binary_search(P, np, Input[i]);
        Output[aux_pos[part]++] = Input[i];
    }

    int *send_counts = malloc(np * sizeof(int));
    int *s_displs = malloc(np * sizeof(int));
    int *recv_counts = malloc(np * sizeof(int));
    int *r_displs = malloc(np * sizeof(int));

    for (int i = 0; i < np; i++) {
        send_counts[i] = aux_p[i];
        s_displs[i] = aux_N[i];
    }

    MPI_Alltoall(send_counts, 1, MPI_INT, recv_counts, 1, MPI_INT, MPI_COMM_WORLD);

    int total_recv_size = 0;
    for (int i = 0; i < np; i++) {
        r_displs[i] = total_recv_size;
        total_recv_size += recv_counts[i];
    }
    *nO = total_recv_size;

    long long *recv_buf = malloc(total_recv_size * sizeof(long long));

    MPI_Alltoallv(Output, send_counts, s_displs, MPI_LONG_LONG, recv_buf, recv_counts, r_displs, MPI_LONG_LONG, MPI_COMM_WORLD);

    free(aux_p);
    free(aux_pos);
    free(aux_N);
    free(send_counts);
    free(s_displs);
    free(recv_counts);
    free(r_displs);
    free(recv_buf);
}

int main(int argc, char **argv) {
    long long nTotalElements;
    chronometer_t multiPartitionMPITime;

    if (argc != 2) {
        printf("usage: %s <nTotalElements>\n", argv[0]);
        return 1;
    }

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(2024 * 100 + rank);

    nTotalElements = atoi(argv[1]);
    int np = size;
    int n = nTotalElements / np;
    int nO;

    long long *Input = (long long *)malloc(n * sizeof(long long));
    long long *P = (long long *)malloc(size * sizeof(long long));
    long long *Output = (long long *)malloc(n * sizeof(long long));

    for (int i = 0; i < n; ++i) {
        Input[i] = geraAleatorioLL();
    }

    if (rank == 0) {
        for (int i = 0; i < np - 1; ++i) {
            P[i] = geraAleatorioLL();
        }
        P[np - 1] = LLONG_MAX;
        qsort(P, size, sizeof(long long), compare_LL);
    }
    MPI_Bcast(P, np, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    chrono_reset(&multiPartitionMPITime);
    chrono_start(&multiPartitionMPITime);

    for(int i = 0; i < NTIMES; i++)
        multi_partition_mpi(Input, n, P, size, Output, &nO);

    chrono_stop(&multiPartitionMPITime);

    verifica_particoes(Input, n, P, np, Output, &nO);

    double total_time_in_seconds = (double)chrono_gettotal(&multiPartitionMPITime) / ((double)1000 * 1000 * 1000);
    printf("total_time_in_seconds: %lf s\n", total_time_in_seconds);

    double OPS = ((double)nTotalElements * NTIMES) / total_time_in_seconds;
    printf("Throughput: %lf OP/s\n", OPS);
    
    free(Input);
    free(P);
    free(Output);

    MPI_Finalize();
    return 0;
}
