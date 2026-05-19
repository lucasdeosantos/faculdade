#include <semaphore.h>
#include <stdlib.h>

typedef struct barrier_s {
    int total;      // número total de processos que devem sincronizar
    int count;      // contador de processos que chegaram
    sem_t mutex;    // protege o count
    sem_t wait_sem; // semáforo que bloqueia processos até todos chegarem
} barrier_t;

void init_barr( barrier_t *barr, int n ) {
    barr->total = n;
    barr->count = 0;

    // semáforo mutex inicia aberto (1)
    sem_init(&barr->mutex, 1, 1);

    // semáforo de espera inicia fechado (0)
    sem_init(&barr->wait_sem, 1, 0);
}

void process_barrier( barrier_t *barr ) {
    // entrar na região crítica
    sem_wait(&barr->mutex);

    barr->count++;

    int pos = barr->count;
    int total = barr->total;

    if (pos == total) {
        // último processo chegou → libera todos os outros
        for (int i = 0; i < total - 1; i++) {
            sem_post(&barr->wait_sem);
        }

        // opcional: resetar a barreira se for usada novamente
        barr->count = 0;

        sem_post(&barr->mutex);
        return;
    }

    // não é o último → libera o mutex
    sem_post(&barr->mutex);

    // espera até que o último processo faça sem_post()
    sem_wait(&barr->wait_sem);
}