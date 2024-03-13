#include <string.h>
#include "ordenacao.h"

void getNome(char nome[]){
	strncpy(nome, "Lucas Emanuel de Oliveira Santos", MAX_CHAR_NOME);
	nome[MAX_CHAR_NOME-1] = '\0';
}

unsigned int getGRR(){
	return 20224379;
}

int auxBuscaSequencial(int vetor[], int a, int b, int valor, int *numComparacoes){
	if(a > b)
		return -1;

	(*numComparacoes)++;
	if(vetor[b] == valor)
		return b;

	return auxBuscaSequencial(vetor,a,b-1,valor,numComparacoes);
}

int buscaSequencial(int vetor[], int tam, int valor, int* numComparacoes){
	return auxBuscaSequencial(vetor,0,tam - 1,valor,numComparacoes);
}

int buscaSequencialGenerica(int vetor[], int a, int b, int valor, int *numComparacoes){
	if(a > b)
		return a - 1;

	(*numComparacoes)++;
	if(valor >= vetor[b])
		return b;

	return buscaSequencialGenerica(vetor,a,b-1,valor,numComparacoes);
}

int auxBuscaBinaria(int vetor[], int a, int b, int valor, int *numComparacoes){
	int m;

	m = (a + b)/2;

	if(a > b){
		if(vetor[m] == valor)
			return m;

		return -1;
	}

	(*numComparacoes)++;
	if(valor < vetor[m])
		return auxBuscaBinaria(vetor,a,m-1,valor,numComparacoes);

	return auxBuscaBinaria(vetor,m+1,b,valor,numComparacoes);
}

int buscaBinaria(int vetor[], int tam, int valor, int* numComparacoes){
	return auxBuscaBinaria(vetor,0,tam-1,valor,numComparacoes);
}

int buscaBinariaGenerica(int vetor[], int a, int b, int valor, int *numComparacoes){
	int m;

	if(a > b)
		return a - 1;

	m = (a + b)/2;

	(*numComparacoes)++;
	if(valor < vetor[m])
		return buscaBinariaGenerica(vetor,a,m-1,valor,numComparacoes);

	return buscaBinariaGenerica(vetor,m+1,b,valor,numComparacoes);
}

void troca(int vetor[], int a, int b){
	int aux = vetor[a];
	vetor[a] = vetor[b];
	vetor[b] = aux;

	return;
}

void insere(int vetor[], int a, int b, int *numComparacoes){
	int p;

	p = buscaBinariaGenerica(vetor,a,b-1,vetor[b],numComparacoes);
	for(int i = b; i > p + 1; i--)
		troca(vetor,i,i-1);

	return;
}

int auxInsertionSort(int vetor[], int a, int b, int *numComparacoes){
	if(a >= b)
		return (*numComparacoes);

	auxInsertionSort(vetor,a,b-1,numComparacoes);
	insere(vetor,a,b,numComparacoes);

	return (*numComparacoes);
}

int insertionSort(int vetor[], int tam){	
	int numComparacoes = 0;

	return auxInsertionSort(vetor,0,tam-1,&numComparacoes);
}

int menor(int vetor[], int a, int b, int *numComparacoes){
	int m = a;

	for(int i = a + 1; i <= b; i++){
		(*numComparacoes)++;
		if(vetor[i] < vetor[m])
			m = i;
	}
	return m;
}

int auxSelectionSort(int vetor[], int a, int b, int *numComparacoes){
	if(a >= b)
		return (*numComparacoes);

	troca(vetor,a,menor(vetor,a,b,numComparacoes));

	return auxSelectionSort(vetor,a+1,b, numComparacoes);
}

int selectionSort(int vetor[], int tam){
	int numComparacoes = 0;

	return auxSelectionSort(vetor,0,tam-1, &numComparacoes);
}

void copiar(int v[], int w[], int a, int b){
	for(int i = a; i <= b; i++)
		v[i] = w[i];

	return;
}

void merge(int vetor[], int a, int m, int b, int *numComparacoes){
	int i, j, k;
	int vetorAuxiliar[b+1];

	if(a >= b)
		return;

	k = a;
	i = a;
	j = m + 1;
	while(i <= m && j <= b){
		(*numComparacoes)++;
		if(vetor[i] < vetor[j])
			vetorAuxiliar[k++] = vetor[i++];
		else
			vetorAuxiliar[k++] = vetor[j++];
	}

	while(i <= m)
		vetorAuxiliar[k++] = vetor[i++];

	while(j <= b)
		vetorAuxiliar[k++] = vetor[j++];

	return copiar(vetor,vetorAuxiliar,a,b);
}

int auxMergeSort(int vetor[], int a, int b, int *numComparacoes){
	int m;
	
	if(a >= b)
		return (*numComparacoes);

	m = (a+b)/2;

	auxMergeSort(vetor,a,m,numComparacoes);
	auxMergeSort(vetor,m+1,b,numComparacoes);
	merge(vetor,a,m,b,numComparacoes);

	return (*numComparacoes);
}

int mergeSort(int vetor[], int tam){
	int numComparacoes = 0;
	
	return auxMergeSort(vetor,0,tam - 1, &numComparacoes);
}

int particionar(int vetor[], int a, int b, int *numComparacoes){
	int pivo = vetor[b];
	int m = a;

	for(int i = a; i < b; i++){
		(*numComparacoes)++;
		if(vetor[i] <= pivo){
			troca(vetor,m,i);
			m++;
		}
	}
	troca(vetor,m,b);

	return m;
}

int auxQuickSort(int vetor[], int a, int b, int *numComparacoes){
	int m;

	if(a >= b)
		return (*numComparacoes);

	m = particionar(vetor,a,b,numComparacoes);
	auxQuickSort(vetor,a,m-1,numComparacoes);
	auxQuickSort(vetor,m+1,b,numComparacoes);

	return (*numComparacoes);
}

int quickSort(int vetor[], int tam){
	int numComparacoes = 0;

	return auxQuickSort(vetor,0,tam - 1, &numComparacoes);
}

int indicePai(int i){
	return i/2;
}

int indiceFilhoEsquerdo(int i){
	return i*2;
}

int indiceFilhoDireito(int i){
	return (i*2) + 1;
}

void maxHeapfy(int h[], int i, int n, int *numComparacoes){
	int l, r, maior;

	maior = i;
	l = indiceFilhoEsquerdo(i);
	r = indiceFilhoDireito(i);

	(*numComparacoes)++;
	if(l < n && h[l] > h[maior])
		maior = l;

	(*numComparacoes)++;
	if(r < n && h[r] > h[maior])
		maior = r;

	if(maior == i)
		return;

	troca(h,i,maior);
	maxHeapfy(h,maior,n,numComparacoes);
}

void construirMaxHeap(int vetor[], int tam, int *numComparacoes){
	for(int i = tam/2 - 1; i >= 0 ;i--)
		maxHeapfy(vetor,i,tam,numComparacoes);

	return;
}

int heapSort(int vetor[], int tam){
	int numComparacoes = 0;

	construirMaxHeap(vetor,tam, &numComparacoes);
	for(int i = tam - 1; i >= 0; i--){
		troca(vetor,0,i);
		maxHeapfy(vetor,0,i,&numComparacoes);
	}

	return numComparacoes;
}
