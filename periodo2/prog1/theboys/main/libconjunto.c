#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"
#define MAX 100 

/* InsertionSort para ordenar o vetor */
conjunto_t *ordena_vetor(conjunto_t *c){
	int i, j, aux;

	for(i = 0; i < (cardinalidade_cjt(c) - 1); i++)
		for(j = i; j >= 0; j--)
			if(c->v[j+1] < c->v[j]){
				aux = c->v[j];
				c->v[j] = c->v[j+1];
				c->v[j+1] = aux;
			}
	return c;
}

conjunto_t *cria_cjt (int max){
	conjunto_t *c;
	int i;

	if((c = malloc(sizeof(conjunto_t))) == NULL)
		return NULL;

	c->max = max;
	c->card = 0;
	
	if((c->v = malloc(sizeof(int)*max)) == NULL)
		return NULL;

	for(i = 0; i < max; i++)
		c->v[i] = 0;

	return c;
}

conjunto_t *destroi_cjt (conjunto_t *c){
	c->card = 0;

	free(c->v);
	c->v = NULL;

	free(c);
	return c = NULL;
}

int vazio_cjt (conjunto_t *c){
	int i;

	if(cardinalidade_cjt(c) == 0)
		return 1;

	for(i = 0; i < cardinalidade_cjt(c); i++)
		if(c->v[i] != 0)
			return 0;

	return 1;
}	

int cardinalidade_cjt (conjunto_t *c){
	return c->card;
}

int insere_cjt (conjunto_t *c, int elemento){
    if(cardinalidade_cjt(c) == c->max)
		return 0;

	if(pertence_cjt(c,elemento))
		return 1;
	
	c->v[cardinalidade_cjt(c)] = elemento;
	c->card++;
	return 1;	
}

int retira_cjt (conjunto_t *c, int elemento){
	int i, j;

    for (i = 0; i < cardinalidade_cjt(c); i++)
	{
        if (c->v[i] == elemento)
		{
			for(j = i; j < cardinalidade_cjt(c); j++)
				c->v[j] = c->v[j+1];

			c->card--;
			return 1;
		}
	}
    return 0;
}

int pertence_cjt (conjunto_t *c, int elemento){
	int i;

	for(i = 0; i < cardinalidade_cjt(c); i++)
		if(c->v[i] == elemento)
			return 1;

	return 0;
}

int contido_cjt (conjunto_t *c1, conjunto_t *c2){
	int i;

	if(cardinalidade_cjt(c1) > cardinalidade_cjt(c2))
		return 0;

	for(i = 0; i < cardinalidade_cjt(c1);i++)
		if(!pertence_cjt(c2,c1->v[i]))
			return 0;
				
	return 1;
}

int sao_iguais_cjt (conjunto_t *c1, conjunto_t *c2){
	int i;
		
	if(cardinalidade_cjt(c1) == cardinalidade_cjt(c2))
	{
		for(i = 0; i < cardinalidade_cjt(c1); i++)
			if(c1->v[i] != c2->v[i])
				return 0;

		return 1;
	}
	return 0;
}

conjunto_t *diferenca_cjt (conjunto_t *c1, conjunto_t *c2){
	int i, j, insere;
	conjunto_t *diferenca;

	if((diferenca = cria_cjt(MAX)) == NULL)
		return NULL;

	for(i = 0; i < cardinalidade_cjt(c1); i++)
	{
		insere = 1;
		for(j = 0; j < cardinalidade_cjt(c2); j++)
			if(c1->v[i] == c2->v[j])
				insere = 0;

		if(insere)
			insere_cjt(diferenca, c1->v[i]);
	}
	return diferenca;
}

conjunto_t *interseccao_cjt (conjunto_t *c1, conjunto_t *c2){
	int i, j, insere;
	conjunto_t *interseccao;

	if((interseccao = cria_cjt(MAX)) == NULL)
		return NULL;

	for(i = 0; i < cardinalidade_cjt(c1); i++)
	{
		insere = 0;
		for(j = 0; j < cardinalidade_cjt(c2); j++)
			if(c1->v[i] == c2->v[j])
				insere = 1;

		if(insere)
			insere_cjt(interseccao, c1->v[i]);
	}
	return interseccao;
}

conjunto_t *uniao_cjt (conjunto_t *c1, conjunto_t *c2){
	int i;
	conjunto_t *uniao;

	if((uniao = cria_cjt(MAX)) == NULL)
		return NULL;

	for(i = 0; i < cardinalidade_cjt(c1); i++)
		insere_cjt(uniao,c1->v[i]);

	for(i = 0; i < cardinalidade_cjt(c2); i++)
		insere_cjt(uniao,c2->v[i]);
	return uniao;
}

conjunto_t *copia_cjt (conjunto_t *c){
	int i;
	conjunto_t *copy;

	if((copy = cria_cjt(MAX)) == NULL)
		return NULL;

	for(i = 0; i < cardinalidade_cjt(c); i++)
		insere_cjt(copy,c->v[i]);

	copy->card = cardinalidade_cjt(c); 
	return copy;
}

conjunto_t *cria_subcjt_cjt (conjunto_t *c, int n){
	int random;
	conjunto_t *subcjt;

	subcjt = copia_cjt(c);

	if(n >= cardinalidade_cjt(c))
		return subcjt;

	while(cardinalidade_cjt(subcjt) > n)
	{
		random = rand() % cardinalidade_cjt(subcjt);
		retira_cjt(subcjt,subcjt->v[random]);
	}
	return subcjt;
}		

void imprime_cjt (conjunto_t *c){
	int i;

	ordena_vetor(c);

	if(vazio_cjt(c))
	{
		printf("conjunto vazio\n");
		return;
	}

	for(i = 0; i < (cardinalidade_cjt(c) - 1); i++)
		printf("%d ", c->v[i]);
	printf("%d", c->v[cardinalidade_cjt(c) - 1]);
	printf("\n");
	return;
}

void inicia_iterador_cjt (conjunto_t *c){
	c->ptr = 0;
	return;
}

int incrementa_iterador_cjt (conjunto_t *c, int *ret_iterator){
	*ret_iterator = c->v[c->ptr++];

	if(c->ptr > cardinalidade_cjt(c))
		return 0;

	return 1;
}

int retira_um_elemento_cjt (conjunto_t *c){
	int r, random = rand() % cardinalidade_cjt(c);

	r = c->v[random];
	retira_cjt(c, c->v[random]);
	return r;
}
