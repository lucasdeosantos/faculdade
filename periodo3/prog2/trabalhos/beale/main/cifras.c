#include "beale.h"

FILE *abreArquivo(char nome[], char modo[]) {

	FILE *arquivo;

	arquivo = fopen(nome, modo);

	if(!arquivo) {
		perror("Erro ao abrir arquivo");
		exit(EXIT_FAILURE);
	}
	return arquivo;
}

int fechaArquivo (FILE *arquivo) {
	return fclose(arquivo);
}

lista_t **criaVetorCifras() {
	lista_t **cifras;

	if(!(cifras = malloc(sizeof(lista_t*)*MAX_SIZE_ARRAY)))
		return NULL;

	for(int i = 0; i < MAX_SIZE_ARRAY; i++)
		cifras[i] = NULL;

	return cifras;
}

lista_t **destroiVetorCifras(lista_t **cifras) {

	for(int i = 0; i < MAX_SIZE_ARRAY; i++) {
		if(cifras[i] != NULL)
			destroiLista(cifras[i]);
	}
	free(cifras);

	return NULL;
}

int insereVetorCifras(lista_t **cifras, int i, long chave) {
	if(i >= 0 && i <= 255) {
		if(cifras[i] == NULL) {
			if(!(cifras[i] = criaLista()))
				return 0;

			if(insereLista(cifras[i], chave))
				return 1;

			return 0;
		}
		if(insereLista(cifras[i], chave))
			return 1;

		return 0;
	}
	return 0;
}

void geraCifras(FILE *LivroCifra, lista_t **cifras) {
	char palavra[MAX_SIZE_WORD];
	long numPalavra = 0;

	while(fscanf(LivroCifra, "%s", palavra) != EOF) {
		insereVetorCifras(cifras, palavra[0], numPalavra);	
		numPalavra++;
	}
}

void geraCifrasArquivo(FILE *ArquivoDeChaves, lista_t **cifras) {
	char palavra[MAX_SIZE_WORD];
	long chave;
	int i = 0;

	while(fscanf(ArquivoDeChaves, "%s", palavra) != EOF) {
		if(palavra[1] == DOISPONTOS)
			i = palavra[0];
		else {
			chave = atol(palavra);
			insereVetorCifras(cifras, i, chave);
		}
	}
	return;
}

void geraArquivoChaves(char nome[], lista_t **cifras) {
	FILE *ArquivoDeChaves;

	ArquivoDeChaves = abreArquivo(nome, "w");

	for(int i = 0; i < MAX_SIZE_ARRAY; i++) {
		if(cifras[i] != NULL){
			fprintf(ArquivoDeChaves, "%c: ", i);
			escreveListaArquivo(ArquivoDeChaves, cifras[i]);
		}
	}
	fechaArquivo(ArquivoDeChaves);
	return;
}
