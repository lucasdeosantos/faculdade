#include "fila.h"

struct fila *criaFila()
{
	struct fila *fila = malloc(sizeof(struct fila));

	if (!fila)
		return NULL;

	fila->ini = NULL;
	fila->fim = NULL;
	fila->tamanho = 0;

	return fila;
}

struct fila *destroiFila(struct fila *fila)
{
	while (!vaziaFila(fila)) {
		struct nodo_f *aux = fila->ini;
		fila->ini = fila->ini->prox;
		free(aux);
	}
	free(fila);
	return NULL;
}

int vaziaFila(struct fila *fila)
{
	if (fila->tamanho)
		return 0;

	return 1;
}

int enfileirar(struct fila *fila, struct nodo *nodo)
{
	if (vaziaFila(fila)) {
		if (!(fila->ini = malloc(sizeof(struct nodo_f))))
			return 0;

		fila->fim = fila->ini;
		fila->ini->nodo = nodo;
	}
	else {
		if (!(fila->fim->prox = malloc(sizeof(struct nodo_f))))
			return 0;

		fila->fim = fila->fim->prox;
		fila->fim->nodo = nodo;
	}
	fila->tamanho++;
	return 1;
}

struct nodo *removerCabeca(struct fila *fila)
{
	if (vaziaFila(fila))
		return NULL;

	struct nodo *nodo = fila->ini->nodo;
	struct nodo_f *aux = fila->ini;

	fila->ini = fila->ini->prox;
	fila->tamanho--;
	free(aux);

	return nodo;
}
