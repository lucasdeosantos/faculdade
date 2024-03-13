#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "libpilha.h"

t_pilha* cria_pilha(int n){
	t_pilha *p;

	if(!(p = malloc(sizeof(t_pilha))))
		return NULL;

	if(!(p->v = malloc(sizeof(double)*n)))
		return NULL;

	p->tam = n;
	p->topo = 0;

	return p;
}

t_pilha* destroi_pilha(t_pilha *p){
	free(p->v);
	free(p);

	return p = NULL;
}

int pilha_vazia(t_pilha *p){
	if(!p->topo || !p)
		return 1;

	return 0;
}

int empilha (double x, t_pilha *p){
	if(p->topo == p->tam || !p)
		return 0;

	p->v[++p->topo] = x;

	return 1;
}

int desempilha(double *t, t_pilha *p){
	if(pilha_vazia(p))
		return 0;

	*t = p->v[p->topo--];

	return 1;
}

int topo(double *t, t_pilha *p){
	if(pilha_vazia(p))
		return 0;

	*t = p->v[p->topo];
	return 1;
}
