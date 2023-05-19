#include "libmundo.h"

int main(){
	mundo_t *mundo;
	evento_t *evento;
	int fim = 0;

	srand(time(NULL));

	mundo = cria_mundo();

	while(!fim){
		evento = obtem_primeiro_lef(mundo->lef);
		mundo->tempoAtual = evento->tempo;

		switch (evento->tipo){
			case CHEGADA:
				tratar_evento_chegada(mundo,evento);
				break;

			case SAIDA:
				tratar_evento_saida(mundo,evento);
				break;

			case MISSAO:
				tratar_evento_missao(mundo,evento);
				break;

			case FIM:
				fim_do_mundo(mundo);
				fim = 1;
				break;
		}
		free(evento);
	}
	return 0;
}
