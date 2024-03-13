#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "vetores.h"

int *criaVetor(int tam){
	int *v;

	if(!(v = malloc(sizeof(int)*tam)))
		return NULL;

	return v;
}

void liberaVetor(int v[]){
	free(v);
	v = NULL;
	return;
}

int leTamanho(){
	int tam;

	scanf("%d", &tam);
	return tam;
}

void leVetor(int v[], int tam){
	for(int i = 0; i < tam; i++)
		scanf("%d", &v[i]);
	return;
}

void vetorAleatorio(int v[], int tam){
	for(int i = 0; i < tam; i++)
		v[i] = rand() % (tam + 1);
	return;
}

void vetorLinear(int v[], int tam){
	for(int i = 0; i < tam; i++)
		v[i] = i;
	return;
}

void vetorInvertido(int v[], int tam){
	for(int i = 0; i < tam; i++)
		v[i] = tam - i - 1;
	return;
}

void vetorVazio(int v[], int tam){
	for(int i = 0; i < tam; i++)
		v[i] = 0;
}

int ordenado(int v[], int tam){
	while (--tam >= 1)
		if (v[tam] < v[tam-1]) 
			return 0;
	
	return 1;
}

void copiaVetor(int v[], int tam, int w[]){
	for(int i = 0; i < tam; i++)
		w[i] = v[i];

	return;
}

void desordenaVetor(int v[], int tam){
	int r, aux;

	for(int i = 0; i < tam; i++){
		aux = v[i];
		r = rand() % tam;
		v[i] = v[r];
		v[r] = aux;
	}
	return;
}

void imprimeVetor(int v[], int tam){
	for(int i = 0; i < tam - 1; i++)
		printf("%d ", v[i]);
	printf("%d\n", v[tam-1]);
	return;
}
