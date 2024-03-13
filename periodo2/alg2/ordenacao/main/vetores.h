/* aloca memória para um vetor de inteiros */
/* retorna NULL se der errado */
int *criaVetor(int tam);

/* libera a memória alocada para um vetor */
void liberaVetor(int v[]);

/* le o tamanho de um vetor */
int leTamanho();

/* le um vetor */
void leVetor(int v[], int tam);

/* cria um vetor aleatório */
void vetorAleatorio(int v[], int tam); 

/* cria um vetor onde o valor de v[i] é igual ao seu indice */
void vetorLinear(int v[], int tam);

/* cria um vetor linear invertido */
void vetorInvertido(int v[], int tam);

/* cria um vetor onde o valor de v[i] é sempre igual a 0 */
void vetorVazio(int v[], int tam);

/* retorna 1 se o vetor estiver ordenado, retorna 0 caso contrário */
int ordenado(int v[], int tam);

/* copia o vetor v para o vetor w */
void copiaVetor(int v[], int tam, int w[]);

/* recebe um vetor ordenado e desordena ele */
void desordenaVetor(int v[], int tam);

/* imprime um vetor */
void imprimeVetor(int v[], int tam);
