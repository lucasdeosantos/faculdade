#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liblef.h"

lef_t *cria_lef(){
	lef_t *l;

	if(!(l = malloc(sizeof(lef_t))))
		return NULL;

	l->Primeiro = NULL;
	return l;
}

lef_t *destroi_lef(lef_t *l){
	nodo_lef_t *aux;

	while(l->Primeiro){
		aux = l->Primeiro;
		l->Primeiro = aux->prox;
		free(aux->evento);
		free(aux);
	}
	free(l);
	return l = NULL;
}

int adiciona_inicio_lef(lef_t *l, evento_t *evento){
	nodo_lef_t *primeiro;
	evento_t *copy;
	
	if(!(primeiro = malloc(sizeof(nodo_lef_t))))
		return 0;

	if(!(copy = malloc(sizeof(evento_t))))
		return 0;

	memcpy(copy,evento,sizeof(evento_t));

	primeiro->evento = copy;
	primeiro->prox = l->Primeiro;
	l->Primeiro = primeiro;
	return 1;
}

int adiciona_ordem_lef (lef_t *l, evento_t *evento){
    nodo_lef_t *novo, *aux1, *aux2;
    evento_t *copy;
    
    if(!(novo = malloc(sizeof(nodo_lef_t))))
        return 0;

    if(!(copy = malloc(sizeof(evento_t))))
        return 0;

    memcpy(copy, evento, sizeof(evento_t));
    
    novo->evento = copy;
    novo->prox = NULL;

	aux1 = l->Primeiro;
	aux2 = l->Primeiro->prox;

    if(!aux1->prox){
        if(aux1->evento->tempo > novo->evento->tempo){
            l->Primeiro = novo;
            novo->prox = aux1;
            return 1;
        }
        aux1->prox = novo;
        return 1;
    }

    while(aux2->prox){
        if(aux1->evento->tempo > novo->evento->tempo){
            l->Primeiro = novo;
            novo->prox = aux1;
            return 1;
        }
        else if((aux2->evento)->tempo > novo->evento->tempo){
            aux1->prox = novo;
            novo->prox = aux2;
            return 1;
        }
        aux1 = aux2;
        aux2 = aux2->prox;
    }
    aux2->prox = novo;
    return 1;
}

evento_t *obtem_primeiro_lef(lef_t *l){
	nodo_lef_t *aux;
	evento_t *evento;

	aux = l->Primeiro;

	if(!aux)
		return NULL;

	l->Primeiro = aux->prox;
	evento = aux->evento;
	free(aux);
	return evento;
}
