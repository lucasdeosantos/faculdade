#include "beale.h"

long geraChaveAleatoria(lista_t *l) {
	nodo_t *aux;

	aux = l->ini;

	for(unsigned int i = 1; i < rand() % l->tamanho; i++)
		aux = aux->prox;

	return aux->chave;
}

int indice(char caracter) {
	return caracter;
}

void geraArquivoCodificado(FILE *MensagemOriginal, char nome[], lista_t **cifras) {
	FILE *MensagemCodificada;
	char caracter;
	int i;

	MensagemCodificada = abreArquivo(nome, "w");

	while((caracter = fgetc(MensagemOriginal)) != EOF) {
		i = indice(caracter);
		
		if(caracter == SPACE) 
			fprintf(MensagemCodificada, "%s ", "-1");
		else if(caracter == LINEFEED)
			fprintf(MensagemCodificada, "%s", "-2\n");
		else if(i < 0 || i > 256 || cifras[i] == NULL) 
			fprintf(MensagemCodificada, "%s ", "-3");
		else 
			fprintf(MensagemCodificada, "%ld ",  geraChaveAleatoria(cifras[i]));
	}

	fechaArquivo(MensagemCodificada);
	return;
}
