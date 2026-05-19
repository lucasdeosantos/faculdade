#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct nodo_s {
    sem_t sem;              // semáforo individual do processo
    struct nodo_s *prox;
} Nodo;

typedef struct fifoQ_s {
    pthread_mutex_t mutex;   // mutex para proteger a fila
    Nodo *ini;              // início da fila
    Nodo *fim;              // fim da fila
} FifoQT;

// Inicializa a fila FIFO
void init_fifoQ(FifoQT *f) {
    pthread_mutex_init(&f->mutex, NULL);
    f->ini = NULL;
    f->fim = NULL;
}

// Processo chamador deve esperar na fila f
void espera(FifoQT *f) {
    Nodo *n = malloc(sizeof(Nodo));

    sem_init(&n->sem, 0, 0); // novo semáforo fechado
    n->prox = NULL;

    // Entra na região crítica
    pthread_mutex_lock(&f->mutex);

    // Enfileira o processo
    if (f->fim == NULL) {
        f->ini = n;
        f->fim = n;
    } else {
        f->fim->prox = n;
        f->fim = n;
    }

    pthread_mutex_unlock(&f->mutex);

    // Aguarda ser liberado
    sem_wait(&n->sem);

    sem_destroy(&n->sem);
    free(n);
}

// Libera o primeiro processo que esteja esperando na fila f
void liberaPrimeiro(FifoQT *f) {
    pthread_mutex_lock(&f->mutex);

    Nodo *n = f->ini;

    if (n != NULL) {
        // remove da fila
        f->ini = n->prox;
        if (f->ini == NULL)
            f->fim = NULL;

        // libera o processo ao qual este nodo pertence
        sem_post(&n->sem);
    }

    pthread_mutex_unlock(&f->mutex);
}