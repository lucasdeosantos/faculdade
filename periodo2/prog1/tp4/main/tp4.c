#include <stdio.h>
#include <stdlib.h>
#include "lib_conjunto.h"
#define MAX 100

int tamanho_do_vetor(conjunto_t **v)
	{
	int i = 1;

	while(!vazio_cjt(v[i]))
		i++;

	return i;
}

conjunto_t *ler_cjt()
{
	conjunto_t *c;
	int i = 0;

	c = cria_cjt(MAX);

	scanf("%d", &c->v[i]);
	while(c->v[i] != 0)
	{
		i++;
		scanf("%d", &c->v[i]);
	}
	c->card = i;
	
	return c;
}

conjunto_t **ler_vetor_cjt()
{
	conjunto_t **v;
	int i = 1;

	v = malloc(MAX * sizeof(conjunto_t*));

	v[i] = ler_cjt();
	while(!vazio_cjt(v[i]))
	{
		i++;
		v[i] = ler_cjt();
	}
	return v;
}

void libera_vetor_cjt(conjunto_t **v)
{
	int i, tam;

	tam = tamanho_do_vetor(v);

	for(i = 1; i <= tam; i++)
		destroi_cjt(v[i]);

	free(v);
	v = NULL;

	return;
}

int main()
{
	conjunto_t **herois, **equipes, *habilidades, *aux;
	int i, j, k, menor, index;

	herois = ler_vetor_cjt();
	habilidades = ler_cjt();
	equipes = ler_vetor_cjt();

	menor = MAX;
	index = 0;
	i = 1;
	while(i < (tamanho_do_vetor(equipes)))
	{
		aux = cria_cjt(MAX);
		j = 0;
		while(equipes[i]->v[j])
		{
			k = 0;
			while(herois[equipes[i]->v[j]]->v[k])
			{
				insere_cjt(aux,herois[equipes[i]->v[j]]->v[k]);
				k++;
			}
			j++;
		}
		if(contido_cjt(habilidades,aux))
			if(cardinalidade_cjt(aux) <= menor)
			{
				menor = aux->card;
				index = i;
			}
		destroi_cjt(aux);
		i++;
	}
	if(index)
		imprime_cjt(equipes[index]);
	else
		printf("NENHUMA\n");

	libera_vetor_cjt(herois);
	destroi_cjt(habilidades);
	libera_vetor_cjt(equipes);

    return 0;
}
