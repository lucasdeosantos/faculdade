#include <stdio.h>
#include <stdlib.h>

typedef struct presente{
	int valor; // valor sentimental do presente
	int peso; // peso do presente
} presente_t;

typedef struct saco{
	int numPresentes; // numero de presentes do saco
	int capacidadeMaxima; // numero maximo de presentes que cabem no saco
	int valorSaco; // valor sentimental do saco
	int pesoSaco; // peso do saco
	int pesoMaximo; // peso maximo do saco
	int *presentes; // vetor que contem os presentes do saco
} saco_t;

/* aloca memoria para a struct presente. */
presente_t *criaPresente(){
	presente_t *presente;

	if(!(presente = malloc(sizeof(presente_t))))
		return NULL;

	return presente;
}

/* libera a memoria previamente alocada para a struct presente. */
void destroiPresente(presente_t *presente){
	free(presente);

	return;
}

/* le os dados de um presente. */
void lerPresente(presente_t *presente){
	scanf("%d %d", &presente->valor, &presente->peso);

	return;
}

/* aloca memoria para um vetor de structs presente. */
presente_t **criaVetorPresentes(int numeroPresentes){
	presente_t **presentes;
	
	if(!(presentes = malloc(sizeof(presente_t*)*(numeroPresentes + 1))))
		return NULL;

	for(int i = 1; i <= numeroPresentes; i++)
		presentes[i] = criaPresente();

	return presentes;
}

/* desaloca a memoria usada no vetor de structs presente. */
void destroiVetorPresentes(presente_t **presentes, int numeroPresentes){
	for(int i = 1; i <= numeroPresentes; i++)
		free(presentes[i]);

	free(presentes);
	return;
}

/* le todos os presentes de um vetor de structs presente. */
void lerVetorPresentes(presente_t **vetorPresentes, int numeroPresentes){
	for(int i = 1; i <= numeroPresentes; i++){
		lerPresente(vetorPresentes[i]);
	}
	return;
}

/* aloca memoria para struct saco. */
saco_t *criaSaco(int numeroPresentes, int pesoMaximo){
	saco_t *saco;

	if(!(saco = malloc(sizeof(saco_t))))
		return NULL;

	if(!(saco->presentes = malloc(sizeof(int)*numeroPresentes)))
		return NULL;

	saco->numPresentes = 0;
	saco->valorSaco = 0;
	saco->pesoSaco = 0;
	saco->capacidadeMaxima = numeroPresentes;
	saco->pesoMaximo = pesoMaximo;

	return saco;
}

/* desaloca a memoria previamente alocado para a struct saco. */
void destroiSaco(saco_t *saco){
	free(saco->presentes);
	free(saco);

	return;
}

/* retorna 1 caso o saco esteja vazio, retorna 0 caso contrario. */
int sacoVazio(saco_t *saco){
	if(saco->numPresentes)
		return 0;

	return 1;
}

/* o vetor de presentes do saco funciona como uma pilha,
 * portanto as funcoes de adiciona e remove seguem a regra FILO. */

/* se possivel adiciona um elemento no saco e retorna 1, do contrario retorna 0. */
int adicionaSaco(saco_t *saco, presente_t *presente, int numeroPresente){
	if(saco->numPresentes == saco->capacidadeMaxima)
		return 0;

	saco->presentes[saco->numPresentes++] = numeroPresente;
	saco->pesoSaco += presente->peso;
	saco->valorSaco += presente->valor;

	return 1;
}

/* se possivel remove um elemento do saco e retorna 1, do contrario retorna 0. */
int removeSaco(saco_t *saco, presente_t *presente){
	if(sacoVazio(saco))
		return 0;

	saco->numPresentes--;
	saco->pesoSaco -= presente->peso;
	saco->valorSaco -= presente->valor;
	return 1;
}

/* copia o primeiro saco passado como parametro para o segundo. */
void copiaSaco(saco_t *saco, saco_t *copia){
	copia->numPresentes = saco->numPresentes; 
	copia->valorSaco = saco->valorSaco; 
	copia->pesoSaco = saco->pesoSaco; 
	copia->pesoMaximo  = saco->pesoMaximo;

	for(int i = 0; i < saco->numPresentes; i++)
		copia->presentes[i] = saco->presentes[i];
}

/* imprime um saco. */
void imprimeSaco(saco_t *saco){
	if(sacoVazio(saco)){
		printf("saco vazio\n");
		return;
	}

	for(int i = 0; i < saco->numPresentes; i++){
		if(i < saco->numPresentes - 1)
			printf("%d ", saco->presentes[i]);
		else
			printf("%d\n", saco->presentes[i]);
	}
	printf("%d\n", saco->valorSaco);

	return;
}

/* os proximos passos sao os proximos presentes que serao testados a partir do presente atual, 
 * retorna o tamanho do vetor, ou seja, o numero de passos. */
int proximosPassos(int v[], int presenteAtual, int numeroPresentes){
	int numeroPassos;

	numeroPassos = numeroPresentes - presenteAtual;

	for(int i = 0, j = presenteAtual; i < numeroPassos; i++, j++)
		v[i] = j + 1;

	return numeroPassos;
}

void auxSolucao(saco_t *saco, saco_t *aux, presente_t **presentes, int presenteAtual){
	int numeroPassos;
	int passos[saco->capacidadeMaxima];

	/* adicona o presente atual no aux e testa se execedeu o peso maximo. */	
	adicionaSaco(aux, presentes[presenteAtual], presenteAtual);

	/* se o peso do saco for > peso maximo, remove o presente atual do aux e testa o proximo presente. */
	if(aux->pesoSaco > saco->pesoMaximo){
		removeSaco(aux, presentes[presenteAtual]);
		return;
	}

	/* testa se a solucao atual é valida, se for, copia a solucao atual para a saco e testa o proximo presente. */
	if(aux->valorSaco > saco->valorSaco){
		copiaSaco(aux, saco);
	}

	/* calcula os proximos passos da solucao atual. */
	numeroPassos = proximosPassos(passos, presenteAtual, saco->capacidadeMaxima);

	/* testa todos os passos do presente atual. */
	for(int i = 0; i < numeroPassos; i++){

		/* chama a solucao para o passo[j] da solucao atual. */
		auxSolucao(saco, aux, presentes, passos[i]);
	}

	/* quando termina de executar os passos do presente atual, remove o mesmo do aux e testa os proximos passos da solucao anterior. */
	removeSaco(aux, presentes[presenteAtual]);
	return;
}

void solucao(saco_t *saco, presente_t **presentes){
	saco_t *aux;

	/* cria um saco auxiliar para fazer modificacoes. */ 
	aux = criaSaco(saco->capacidadeMaxima, saco->pesoMaximo);

	/* testa a solucao para todos os presentes. */
	for(int i = 1; i <= saco->capacidadeMaxima; i++)
		auxSolucao(saco, aux, presentes, i);
		
	/* desaloca a memoria usada no vetor auxiliar */
	destroiSaco(aux);
}

int main() {
	saco_t *saco;
	presente_t **presentes;
	int numeroPresentes, pesoMaximo;
	
	scanf("%d %d", &numeroPresentes, &pesoMaximo);
		
	presentes = criaVetorPresentes(numeroPresentes);
	lerVetorPresentes(presentes, numeroPresentes);

	saco = criaSaco(numeroPresentes, pesoMaximo);

	solucao(saco, presentes);

	imprimeSaco(saco);

	destroiSaco(saco);
	destroiVetorPresentes(presentes, numeroPresentes);

	return 0;
}
