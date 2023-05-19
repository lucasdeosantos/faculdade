#include "avl.h"

#ifndef FILA_H_
#define FILA_H_ 

struct nodo_f {
	struct nodo *nodo;
	struct nodo_f *prox;
};

struct fila {
	struct nodo_f *ini;
	struct nodo_f *fim;
	int tamanho;
};

// aloca memoria para uma fila.
// retorna NULL em caso de erro na alocacao de memoria.
struct fila *criaFila();

// desaloca a memoria utilizada para uma fila.
struct fila *destroiFila(struct fila *fila);

// retorna 1 se a fila estiver vazia, retorna 0 caso contrario.
int vaziaFila(struct fila *fila);

// enfileira um nodo em uma fila.
int enfileirar(struct fila *fila, struct nodo *nodo);

// remove e retorna o primeiro elemento de uma fila.
// retorna NULL se a fila estiver vazia.
struct nodo *removerCabeca(struct fila *fila);

#endif // FILA_H_
