#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 100

typedef struct {
    int data[MAX];
    int top;
    sem_t mutex;
} stack_t;

void init_stack(stack_t *s) {
    s->top = -1;
    sem_init(&s->mutex, 0, 1);
}

void push(stack_t *s, int value) {
    sem_wait(&s->mutex);

    if (s->top < MAX - 1) {
        s->data[++(s->top)] = value;
        printf("Push: %d\n", value);
    } 
    else {
        printf("Erro: pilha cheia\n");
    }

    sem_post(&s->mutex);
}

int pop(stack_t *s) {
    int value = -1;

    sem_wait(&s->mutex);

    if (s->top >= 0) {
        value = s->data[(s->top)--];
        printf("Pop: %d\n", value);
    } 
    else {
        printf("Erro: pilha vazia\n");
    }

    sem_post(&s->mutex);

    return value;
}

void *thread_push(void *arg) {
    stack_t *s = (stack_t*)arg;
    for (int i = 0; i < MAX; i++) 
        push(s, i);

    return NULL;
}

void *thread_pop(void *arg) {
    stack_t *s = (stack_t*)arg;
    for (int i = 0; i < MAX; i++)
        pop(s);

    return NULL;
}

int main() {
    stack_t s;
    init_stack(&s);

    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread_push, &s);
    pthread_create(&t2, NULL, thread_pop, &s);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}