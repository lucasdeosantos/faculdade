#ifndef ORDENACAO_H_
#define ORDENACAO_H_

#define MAX_CHAR_NOME 50

//a função getNome deve colocar o seu nome dentro da chamada
//seu nome pode ter no máximo MAX_CHAR_NOME - 1 caracteres
void getNome(char nome[]);

//a função a seguir deve retornar o seu número de GRR
unsigned int getGRR();

//Assuma que os vetores estão ordenados ao chamar as funções de busca
//As funções de busca não devem alterar o vetor
//O retorno das funções de busca é o índice do elemento no vetor,
//ou -1 caso o elemeno não exista no vetor
//Você deve armazenar no ponteiro int* numComparacoes o número de comparações
//envolvendo elementos do vetor feito pelo seu algoritmo na chamada

//a busca sequencial é a busca ingênua
int buscaSequencial(int vetor[], int tam, int valor, int* numComparacoes);

//busca binária no vetor
int buscaBinaria(int vetor[], int tam, int valor, int* numComparacoes);

//Cada uma das funções deve ordenar o vetor passado como parâmetro
//O retorno das funções de ordenação é o número de comparações
//entre elementos do vetor

int insertionSort(int vetor[], int tam);

int selectionSort(int vetor[], int tam);

int mergeSort(int vetor[], int tam);

int quickSort(int vetor[], int tam);

int heapSort(int vetor[], int tam);

#endif // ORDENACAO_H_
