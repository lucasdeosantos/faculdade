#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ordenacao.h"
#include "vetores.h"

int main(){
	char nome[MAX_CHAR_NOME];
	int idxBusca;
	int numComp;
	int elemento;
	int tamVetor;
	int *vetor, *vetorCopia;

	srand(time(NULL));

	getNome(nome);
	printf("Trabalho de %s\n", nome);
	printf("GRR %u\n", getGRR());
	printf("Digite o tamanho do vetor: ");
	tamVetor = leTamanho();

	if(!(vetor = criaVetor(tamVetor))){
		printf("Falha fatal. Impossível alocar memoria.\n");
		return 1;
	}
	
	if(!(vetorCopia = criaVetor(tamVetor))){
		printf("Falha fatal. Impossível alocar memoria.\n");
		return 1;
	}

	/* vetorLinear(vetor,tamVetor); */
	/* vetorInvertido(vetor,tamVetor); */
	/* desordenaVetor(vetor,tamVetor); */
	vetorAleatorio(vetor,tamVetor);
	/* vetorVazio(vetor,tamVetor); */
	copiaVetor(vetor,tamVetor,vetorCopia);

	clock_t start, end;
	double total;

	start = clock();
	numComp = insertionSort(vetorCopia, tamVetor);
	end = clock();
	total = ((double)end - start)/CLOCKS_PER_SEC;
	printf("\nTempo total do InsertionSort: %f\n", total);
	printf("Número de comparações do InsertionSort: %d\n", numComp);

	if(!ordenado(vetorCopia,tamVetor))
		printf("Não está ordenado");

	copiaVetor(vetor,tamVetor,vetorCopia);

	start = clock();
	numComp = selectionSort(vetorCopia, tamVetor);
	end = clock();
	total = ((double)end - start)/CLOCKS_PER_SEC;
	printf("\nTempo total do SelectionSort: %f\n", total);
	printf("Número de comparações do SelectionSort: %d\n", numComp);

	if(!ordenado(vetorCopia,tamVetor))
		printf("Não está ordenado");

	copiaVetor(vetor,tamVetor,vetorCopia);

	start = clock();
	numComp = mergeSort(vetorCopia, tamVetor);
	end = clock();
	total = ((double)end - start)/CLOCKS_PER_SEC;
	printf("\nTempo total do MergeSort: %f\n", total);
	printf("Número de comparações do MergeSort: %d\n", numComp);

	if(!ordenado(vetorCopia,tamVetor))
		printf("Não está ordenado");

	copiaVetor(vetor,tamVetor,vetorCopia);

	start = clock();
	numComp = quickSort(vetorCopia, tamVetor);
	end = clock();
	total = ((double)end - start)/CLOCKS_PER_SEC;
	printf("\nTempo total do QuickSort: %f\n", total);
	printf("Número de comparações do QuickSort: %d\n", numComp);

	if(!ordenado(vetorCopia,tamVetor))
		printf("Não está ordenado");

	copiaVetor(vetor,tamVetor,vetorCopia);

	start = clock();
	numComp = heapSort(vetorCopia, tamVetor);
	end = clock();
	total = ((double)end - start)/CLOCKS_PER_SEC;
	printf("\nTempo total do HeapSort: %f\n", total);
	printf("Número de comparações do HeapSort: %d\n", numComp);

	if(!ordenado(vetorCopia,tamVetor))
		printf("Não está ordenado");

	elemento = rand() % (tamVetor + 1);

	numComp = 0;
	start = clock();
	idxBusca = buscaSequencial(vetorCopia, tamVetor, elemento, &numComp);
	end = clock();
	total = ((double)end - start)/CLOCKS_PER_SEC;
	if(elemento != vetorCopia[idxBusca] && idxBusca != -1)
		printf("\nEncontrou o elemento errado.\n");
	else{
		printf("\nTempo total da Busca Sequencial: %f\n", total);
		printf("Número de comparações da Busca Sequencial: %d\n", numComp);
	}

	numComp = 0;
	start = clock();
	idxBusca = buscaBinaria(vetorCopia, tamVetor, elemento, &numComp);
	end = clock();
	total = ((double)end - start)/CLOCKS_PER_SEC;
	if(elemento != vetorCopia[idxBusca] && idxBusca != -1)
		printf("Encontrou o elemento errado.\n");
	else{
		printf("\nTempo total da Busca Binária: %f\n", total);
		printf("Número de comparações da Busca Binária: %d\n", numComp);
	}

	liberaVetor(vetor);
	liberaVetor(vetorCopia);

	return 0;
}
