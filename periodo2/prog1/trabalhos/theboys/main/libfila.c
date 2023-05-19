#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

fila_t *cria_fila(){
	fila_t *f;

	if(!(f = malloc(sizeof(fila_t))))
		return NULL;

	f->ini = NULL;
	f->fim = NULL;
	f->tamanho = 0;
	return f;
}

fila_t *destroi_fila(fila_t *f){
	nodo_f_t *aux;

	while(!vazia_fila(f)){
		aux = f->ini;
		f->ini = f->ini->prox;
		free(aux);
	}
	free(f);
	return f = NULL;
}

int vazia_fila(fila_t *f){
	if(!tamanho_fila(f))
		return 1;

	return 0;
}

int tamanho_fila(fila_t *f){
	return f->tamanho;
}

int insere_fila(fila_t *f, int elemento){
	if(vazia_fila(f)){
		if(!(f->ini = malloc(sizeof(nodo_f_t))))
			return 0;

		f->fim = f->ini;
		f->ini->elem = elemento;
	}
	else {
		if(!(f->fim->prox = malloc(sizeof(nodo_f_t))))
			return 0;

		f->fim = f->fim->prox;
		f->fim->elem = elemento;
	}
	f->tamanho++;
	return 1;
}

int retira_fila(fila_t *f, int *elemento){
	nodo_f_t *aux;

	if(vazia_fila(f))
		return 0;

	aux = f->ini;

	*elemento = f->ini->elem;
	f->ini = f->ini->prox;
	f->tamanho--;
	free(aux);
	return 1;
}

void imprime_fila(fila_t *f){
	nodo_f_t *aux;

	aux = f->ini;

	while(aux != f->fim){
		printf("%d ", aux->elem);
		aux = aux->prox;
	}
	printf("\n");

	return;
}
