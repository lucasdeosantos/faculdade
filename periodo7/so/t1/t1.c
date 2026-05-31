#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <time.h>

#define MAX_PROCS 100 // Limite máximo seguro de processos para a fila FIFO

// --------------------- ESTRUTURAS DE DADOS -------------------------------

// Estrutura da Barreira
typedef struct barrier_s {
    int n;             // Total de processos
    int count;         // Contador de processos que chegaram
    sem_t mutex;       // Protege a variável count
    sem_t turnstile1;  // Catraca da primeira fase
    sem_t turnstile2;  // Catraca da segunda fase
} barrier_t;

// Estrutura da Fila FIFO para uso exclusivo
typedef struct fifoQ_s {
    sem_t mutex;           // Protege as variáveis de controle da fila
    sem_t slots[MAX_PROCS];// Array de semáforos para bloqueio individual (ordem estrita)
    int head;              // Aponta para o próximo a ser acordado (início)
    int tail;              // Aponta para o próximo slot livre na fila (fim)
    int em_uso;            // Flag: 1 se o recurso está sendo usado, 0 se livre
} FifoQT;

// Memória Compartilhada global agregando tudo
typedef struct {
    barrier_t barr;
    FifoQT fifo;
} MemCompartilhada;

// --------------------- FUNÇÕES DA BARREIRA (PARTE A) ---------------------

void init_barr(barrier_t *barr, int n) {
    barr->n = n;
    barr->count = 0;
    // O segundo argumento '1' indica que o semáforo será compartilhado entre processos
    sem_init(&barr->mutex, 1, 1);
    sem_init(&barr->turnstile1, 1, 0);
    sem_init(&barr->turnstile2, 1, 1); // Inicializa aberto para a fase 2
}

void process_barrier(barrier_t *barr) {
    // Fase 1: Chegada
    sem_wait(&barr->mutex);
    barr->count++;
    if (barr->count == barr->n) {
        sem_wait(&barr->turnstile2); // Trava a segunda catraca
        sem_post(&barr->turnstile1); // Libera a primeira
    }
    sem_post(&barr->mutex);

    sem_wait(&barr->turnstile1);
    sem_post(&barr->turnstile1); // Efeito cascata: um libera o próximo

    // Fase 2: Saída
    sem_wait(&barr->mutex);
    barr->count--;
    if (barr->count == 0) {
        sem_wait(&barr->turnstile1); // Trava a primeira catraca para o próximo uso
        sem_post(&barr->turnstile2); // Libera a segunda
    }
    sem_post(&barr->mutex);

    sem_wait(&barr->turnstile2);
    sem_post(&barr->turnstile2); // Efeito cascata
}

// --------------------- FUNÇÕES DA FILA FIFO (PARTE B) --------------------

void init_fifoQ(FifoQT *F) {
    sem_init(&F->mutex, 1, 1);
    F->head = 0;
    F->tail = 0;
    F->em_uso = 0;
    for(int i = 0; i < MAX_PROCS; i++) {
        sem_init(&F->slots[i], 1, 0); // Semáforos individuais iniciam bloqueados
    }
}

void inicia_uso(int recurso, FifoQT *F) {
    sem_wait(&F->mutex);
    
    // Se não há ninguém usando e a fila está vazia
    if (F->em_uso == 0 && F->head == F->tail) {
        F->em_uso = 1;
        sem_post(&F->mutex);
    } else {
        // Recurso ocupado: entra na fila FIFO
        int meu_ticket = (F->tail) % MAX_PROCS;
        F->tail++;
        sem_post(&F->mutex);
        
        // Fica esperando no SEU semáforo exclusivo
        sem_wait(&F->slots[meu_ticket]);
    }
}

void termina_uso(int recurso, FifoQT *F) {
    sem_wait(&F->mutex);
    
    // Se tem alguém esperando na fila
    if (F->head < F->tail) {
        int proximo = (F->head) % MAX_PROCS;
        F->head++;
        // Acorda o processo mais antigo da fila
        sem_post(&F->slots[proximo]);
    } else {
        // Ninguém na fila, apenas libera o recurso
        F->em_uso = 0;
    }
    
    sem_post(&F->mutex);
}

// ------------------- LÓGICA DO PROCESSO (Filhos e Pai) -------------------

void rotina_processo(MemCompartilhada *shm, int nProc, int recurso, int n) {
    // Semente aleatória única por processo para tempos diferentes
    srand(time(NULL) ^ (getpid() << 16)); 

    printf("PID: %d | Pai PID: %d | nProc (lógico): %d\n", getpid(), getppid(), nProc);

    // Sleep aleatório antes da primeira barreira [0, n-1]
    int ns = rand() % n;
    printf("Processo: %d ira dormir %d segundos antes da barreira.\n", nProc, ns);
    sleep(ns);

    // BARREIRA 1
    printf("--Processo: %d chegando na barreira\n", nProc);
    process_barrier(&shm->barr);
    printf("**Processo: %d saindo da barreira\n", nProc);

    // LÓGICA DO RECURSO (Loop de uso FIFO)
    for (int uso = 0; uso < 3; uso++) {
        // (A) Prólogo
        int s = rand() % 4; // 0 a 3 segundos
        printf("Processo: %d Prologo: %d de %d segundos\n", nProc, uso, s);
        sleep(s);
        
        inicia_uso(recurso, &shm->fifo);
        
        // (B) Região Crítica (Uso exclusivo)
        s = rand() % 4;
        printf("Processo: %d USO: %d por %d segundos\n", nProc, uso, s);
        sleep(s);
        
        termina_uso(recurso, &shm->fifo);

        // (C) Epílogo
        s = rand() % 4;
        printf("Processo: %d Epilogo: %d de %d segundos\n", nProc, uso, s);
        sleep(s);
    }

    // BARREIRA 2
    printf("--Processo: %d chegando novamente na barreira\n", nProc);
    process_barrier(&shm->barr);
    printf("++Processo: %d saindo da barreira novamente\n", nProc);
    
}

// ---------------------------------- MAIN ---------------------------------

int main(int argc, char *argv[]) {
    // Checando uso correto
    if (argc != 2) {
        printf("Uso: %s <numero_de_processos>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);
    if (n < 2 || n > MAX_PROCS) {
        printf("O numero de processos deve ser entre 2 e %d\n", MAX_PROCS);
        exit(1);
    }

    // Sorteia um número aleatorio de recurso que vai ser herdado pelos filhos
    srand(time(NULL));
    int recurso = rand() % 1000 + 1;

    // Aloca a memória compartilhada no SO usando mmap
    MemCompartilhada *shm = mmap(NULL, sizeof(MemCompartilhada), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (shm == MAP_FAILED) {
        perror("Erro no mmap");
        exit(1);
    }

    // Processo 0 inicializa estruturas
    init_barr(&shm->barr, n);
    init_fifoQ(&shm->fifo);

    // Array para o pai mapear PID -> nProc
    pid_t *pids_filhos = malloc(n * sizeof(pid_t));
    pids_filhos[0] = getpid(); // nProc 0 é o próprio pai

    // Criação dos n-1 filhos
    for (int i = 1; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Erro no fork");
            exit(1);
        } else if (pid == 0) {
            // Código executado apenas pelo FILHO
            rotina_processo(shm, i, recurso, n);
            exit(0);
        } else {
            // Código do PAI (registra o PID do filho criado)
            pids_filhos[i] = pid;
        }
    }
    
    // Executando a rotina para o pai
    rotina_processo(shm, 0, recurso, n); // Executa como nProc = 0
    
    // APENAS O MAIN ORIGINAL FICA AQUI ESPERANDO (Gerenciador)
    int status;
    pid_t wpid;

    // Espera por TODOS os processos
    while ((wpid = wait(&status)) > 0) {
        // Descobre qual nProc logico corresponde a este PID
        int nProc_logico = -1;
        for (int i = 0; i < n; i++) {
            if (pids_filhos[i] == wpid) {
                nProc_logico = i;
                break;
            }
        }
        if(nProc_logico != -1){
            printf("+++ Filho de número lógico %d e pid %d terminou!\n", nProc_logico, wpid);
        }
    }

    // Limpeza da memória compartilhada
    munmap(shm, sizeof(MemCompartilhada));
    free(pids_filhos);

    return 0;
}