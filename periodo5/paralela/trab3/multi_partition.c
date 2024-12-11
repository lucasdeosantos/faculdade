//
// By Lucas Emanuel de Oliveira Santos
//
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <limits.h>
#include <string.h>
#include "verifica_particoes.h"

typedef long long ll;

ll geraAleatorioLL() {
      int a = rand();
      int b = rand();
      long long v = (long long)a * 100 + b;
      return v;
}

void multi_partition_mpi(ll *Input, int n, ll *P, int np, ll *Output, int *nO) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (np != size) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int *local_counts = (int *)calloc(np, sizeof(int));
    ll **local_buckets = (ll **)malloc(np * sizeof(ll *));
    for (int i = 0; i < np; ++i) {
        local_buckets[i] = (ll *)malloc(n * sizeof(ll));
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < np; ++j) {
            if (Input[i] < P[j]) {
                local_buckets[j][local_counts[j]++] = Input[i];
                break;
            }
        }
    }

    int *global_counts = (int *)malloc(np * sizeof(int));
    MPI_Allreduce(local_counts, global_counts, np, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    int *displs = (int *)malloc(np * sizeof(int));
    int *recv_counts = (int *)malloc(np * sizeof(int));
    for (int i = 0; i < np; ++i) {
        displs[i] = (i == 0) ? 0 : displs[i - 1] + global_counts[i - 1];
        recv_counts[i] = global_counts[i];
    }

    ll *sendbuf = (ll *)malloc(n * sizeof(ll));
    int send_offset = 0;
    for (int i = 0; i < np; ++i) {
        memcpy(sendbuf + send_offset, local_buckets[i], local_counts[i] * sizeof(ll));
        send_offset += local_counts[i];
    }

    ll *recvbuf = (ll *)malloc(global_counts[rank] * sizeof(ll));
    MPI_Alltoallv(sendbuf, local_counts, displs, MPI_LONG_LONG, recvbuf, recv_counts, displs, MPI_LONG_LONG, MPI_COMM_WORLD);

    memcpy(Output, recvbuf, recv_counts[rank] * sizeof(ll));
    *nO = recv_counts[rank];

    free(local_counts);
    for (int i = 0; i < np; ++i) {
        free(local_buckets[i]);
    }
    free(local_buckets);
    free(global_counts);
    free(displs);
    free(recv_counts);
    free(sendbuf);
    free(recvbuf);

    verifica_particoes(Input, n, P, np, Output, nO);
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(2024 * 100 + rank);

    int nTotalElements = (argc > 1) ? atoi(argv[1]) : 1000000;
    int n = nTotalElements / size;

    ll *Input = (ll *)malloc(n * sizeof(ll));
    for (int i = 0; i < n; ++i) {
        Input[i] = geraAleatorioLL();
    }

    ll *P = (ll *)malloc(size * sizeof(ll));
    for (int i = 0; i < size - 1; ++i) {
        P[i] = geraAleatorioLL();
    }
    P[size - 1] = LLONG_MAX;
    qsort(P, size, sizeof(ll), (int (*)(const void *, const void *))strcmp);

    ll *Output = (ll *)malloc(n * sizeof(ll));
    int nO;

    multi_partition_mpi(Input, n, P, size, Output, &nO);

    free(Input);
    free(P);
    free(Output);

    MPI_Finalize();
    return 0;
}
