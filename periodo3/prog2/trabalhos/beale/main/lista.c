#include "beale.h"

nodo_t *criaNodo(long chave) {
	nodo_t *nodo;

	if(!(nodo = malloc(sizeof(nodo_t))))
		return NULL;

	nodo->chave = chave;
	nodo->prox = NULL;

	return nodo;
}

nodo_t *destroiNodo(nodo_t *nodo) {
	free(nodo);

	return NULL;
}

lista_t *criaLista() {
	lista_t *l;

	if(!(l = malloc(sizeof(lista_t))))
		return NULL;

	l->ini = NULL;
	l->tamanho = 0;

	return l;
}

lista_t *destroiLista(lista_t *l) {
	nodo_t *aux;

	while(l->ini != NULL) {
		aux = l->ini->prox;
		destroiNodo(l->ini);
		l->ini = aux;
	}
	free(l);

	return NULL;
}

int insereLista(lista_t *l, long chave) {
	nodo_t *novo;
	
	if(!(novo = criaNodo(chave)))
		return 0;

	if(l->ini == NULL) {
		l->ini = novo;
	}
	else {
		nodo_t *aux;
		aux = l->ini;

		while(aux->prox != NULL)
			aux = aux->prox;
		
		aux->prox = novo;
	}

	l->tamanho++;
	return 1;
}

void escreveListaArquivo(FILE *arquivo, lista_t *l) {
	nodo_t *aux;

	aux = l->ini;

	while(aux != NULL) {
		fprintf(arquivo, "%ld ", aux->chave);
		aux = aux->prox;
	}
	fprintf(arquivo, "%s", "\n");

	return;
}
