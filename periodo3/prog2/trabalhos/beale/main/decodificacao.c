#include "beale.h"

char caracterChave(lista_t **cifras, long chave) {
	if(chave == -1)
		return SPACE;
	else if(chave == -2)
		return LINEFEED;
	else if(chave == -3)
		return HASHTAG;
	else {
		for(int i = 0; i < MAX_SIZE_ARRAY; i++) {
			if(cifras[i] != NULL) {
				nodo_t *aux = cifras[i]->ini;
				while(aux != NULL && aux->chave <= chave) {
					if(aux->chave == chave) {
						return i;
					}
					aux = aux->prox;
				}
			}
		}
		return HASHTAG;
	}
}

void geraArquivoDecodificado(FILE *MensagemCodificada, char nome[], lista_t **cifras) {
	FILE *MensagemDecodificada;
	long chave;

	MensagemDecodificada = abreArquivo(nome, "w");

	while(fscanf(MensagemCodificada, "%ld", &chave) == 1) {
		fprintf(MensagemDecodificada, "%c", caracterChave(cifras, chave));
	}
	fechaArquivo(MensagemDecodificada);
	return;
}
