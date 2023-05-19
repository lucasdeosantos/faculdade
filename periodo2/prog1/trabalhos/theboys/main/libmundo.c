#include <math.h>
#include <stdio.h>
#include "libmundo.h"

/* retorna um numero aleatorio entre a e b. */
int aleat(int a, int b){

	return (rand() % (b - a + 1)) + a;
}

/* retorna o conjunto de todas as habilidades existentes na simulacao. */
conjunto_t *cria_cjt_habilidades_mundo(){
	int i;
	conjunto_t *habilidades;

	if(!(habilidades = cria_cjt(N_HABILIDADES)))
		return NULL;

	for(i = 0; i < N_HABILIDADES; i++)
		insere_cjt(habilidades, i);

	return habilidades;
}

/* aloca memoria e inicia um heroi. */
heroi_t *cria_heroi(mundo_t *mundo, int i){
	heroi_t *heroi;

	if(!(heroi = malloc(sizeof(heroi_t))))
		return NULL;

	heroi->idHeroi = i;
	heroi->paciencia = aleat(0, 100);
	heroi->idade = aleat(18, 100);
	heroi->experiencia = 0;

	if(!(heroi->habilidades = cria_subcjt_cjt(mundo->habilidades,aleat(2, 5))))
		return NULL;

	return heroi;
}

/* aloca memoria para um vetor de herois. */
heroi_t **cria_vetor_herois(mundo_t *mundo){
	int i;
	heroi_t **herois;

	if(!(herois = malloc(sizeof(heroi_t*)*N_HEROIS)))
		return NULL;

	for(i = 0; i < N_HEROIS; i++){
		herois[i] = cria_heroi(mundo, i);
	}
	return herois;
}

/* desaloca toda a memoria usada no vetor de herois. */
heroi_t **destroi_vetor_herois(heroi_t **herois){
	int i;

	for(i = 0; i < N_HEROIS; i++){
		destroi_cjt(herois[i]->habilidades);
		free(herois[i]);
	}

	free(herois);

	return NULL;
}

/* aloca memoria e inicia um local. */
local_t *cria_local(mundo_t *mundo, int i){
	local_t *local;

	if(!(local = malloc(sizeof(local_t))))
		return NULL;

	local->idLocal = i;
	local->lotacaoMaxima = aleat(5, 30);
	local->localizacao.x = aleat(0, N_TAMANHO_MUNDO - 1);
	local->localizacao.y = aleat(0, N_TAMANHO_MUNDO - 1);

	if(!(local->herois = cria_cjt(local->lotacaoMaxima + 1)))
		return NULL;
	if(!(local->fila = cria_fila()))
		return NULL;

	return local;
}

/* aloca memoria para um vetor de locais. */
local_t **cria_vetor_locais(mundo_t *mundo){
	int i;
	local_t **locais;

	if(!(locais = malloc(sizeof(local_t*)*N_LOCAIS)))
		return NULL;

	for(i = 0; i < N_LOCAIS; i++){
		locais[i] = cria_local(mundo, i);
	}	
	return locais;
}

/* desaloca toda a memoria usada no vetor de locais. */
local_t **destroi_vetor_locais(local_t **locais){
	int i; 

	for(i = 0; i < N_LOCAIS; i++){
		destroi_cjt(locais[i]->herois);
		destroi_fila(locais[i]->fila);
		free(locais[i]);
	}

	free(locais);

	return NULL;
}

/* cria um evento com base nos dados passados como parametro. */ 
/* eventos de chegada e saida: dado1 é o id do heroi e o dado2 é o id do local. */
/* evento missao: dado1 é o id da missao. */
evento_t *cria_evento(int tipo, int idHeroi, int idLocal, int tempo){
	evento_t *evento;

	evento = malloc(sizeof(evento_t));

	evento->tipo = tipo;
	evento->dado1 = idHeroi;
	evento->dado2 = idLocal;
	evento->tempo = tempo;

	return evento;
}

/* cria as chegadas iniciais dos herois. */
void cria_chegadas(mundo_t *mundo){
	int i = 0;
	evento_t *evento;

	for(i = 0; i < N_HEROIS; i++){
		evento = cria_evento(CHEGADA, i, aleat(0, N_LOCAIS - 1), aleat(0, 96*7));
		adiciona_ordem_lef(mundo->lef, evento);
		free(evento);
	}
	return;
}

/* cria todas as missoes do mundo. */
void cria_missoes(mundo_t *mundo){
	int i = 0;
	evento_t *evento;

	for(i = 0; i < N_MISSOES; i++){
		evento = cria_evento(MISSAO, i, 0, aleat(mundo->tempoAtual, FIM_DO_MUNDO));
		adiciona_ordem_lef(mundo->lef, evento);
		free(evento);
	}
	return;
}

void cria_fim_do_mundo(mundo_t *mundo){
	evento_t *fim;

	fim = cria_evento(FIM, 0, 0, FIM_DO_MUNDO);
	adiciona_inicio_lef(mundo->lef,fim);
	free(fim);
	return;
}

/* aloca memoria para o mundo e inicia a simulacao. */
mundo_t *cria_mundo(){
	mundo_t *mundo;

	if(!(mundo = malloc(sizeof(mundo_t))))
		return NULL;

	mundo->tempoAtual = 0;
	mundo->nHerois = N_HEROIS;
	mundo->nLocais = N_LOCAIS;
	mundo->tamanhoMundo = N_TAMANHO_MUNDO;

	if(!(mundo->habilidades = cria_cjt_habilidades_mundo()))
		return NULL;
	if(!(mundo->herois = cria_vetor_herois(mundo)))
		return NULL;
	if(!(mundo->locais = cria_vetor_locais(mundo)))
		return NULL;
	if(!(mundo->lef = cria_lef()))
		return NULL;
	
	cria_fim_do_mundo(mundo);
	cria_chegadas(mundo);
	cria_missoes(mundo);

	return mundo;
}

/* retorna a distancia do local a para o local b. */
double distancia(mundo_t *mundo, int a, int b){
	double result;

	result = sqrt(pow((mundo->locais[b]->localizacao.x - mundo->locais[a]->localizacao.x), 2) + pow((mundo->locais[b]->localizacao.y - mundo->locais[a]->localizacao.y), 2));

	return result;
}

/* retorna o maior numero entre a e b. */
int max(int a, int b){
	if(a > b)
		return a;

	return b;
}

/* retorna o tempo de deslocamento do local x para o local y. */
int tdl(mundo_t *mundo, int heroi, int x, int y){
	int result;

	result = distancia(mundo, x, y) / (100 - max(0, mundo->herois[heroi]->paciencia - 40));

	return result;
}

/* se a missao for possivel, retorna o local onde a missao sera realizada, se a missao for impossivel, retorna -1. */
int id_local_equipe_escolhida(mundo_t *mundo, evento_t *evento, conjunto_t *missao){
	int i, j, k, h, menor, local;
	int idMissao = evento->dado1;
	conjunto_t *aux;

	menor = N_HEROIS;
	local = -1;

	/* passa por todos os locais do mundo. */
	for(i = 0; i < N_LOCAIS; i++){

		/* cria um conjunto auxiliar para colocar as habilidades de todos os herois do local i. */
		aux = cria_cjt(N_HEROIS);

		/* passa por todos os herois que estão no local i. */
		for(j = 0; j < cardinalidade_cjt(mundo->locais[i]->herois); j++){

			/* h é o heroi que esta na posicao j do local i. */
			h = mundo->locais[i]->herois->v[j];
			k = 0;

			/* insere todas as habilidades k do heroi h no conjunto aux. */
			for(k = 0; k < cardinalidade_cjt(mundo->herois[k]->habilidades); k++){
				insere_cjt(aux, mundo->herois[h]->habilidades->v[k]);
			}
		}
		/* imprime as habilidades da equipe do local i. */
		printf("%6d:MISSAO %2d HAB_EQL %d:", mundo->tempoAtual, idMissao, i);
		imprime_cjt(aux);

		/* se o conjunto missao estiver contido no conjunto aux. */
		if(contido_cjt(missao,aux))

			/* se a cardinalidade do conjunto aux for a menor cardinalidade de todos os conjuntos que a missao esta contida. */
			if(cardinalidade_cjt(aux) <= menor){
				menor = cardinalidade_cjt(aux);
				local = i;
		}
		destroi_cjt(aux);
	}
	return local;
}

/* atualiza o xp de todos os herois do local passado como parametro. */
void atualizar_xp(mundo_t *mundo, int local){
	int i;

	for(i = 0; i < cardinalidade_cjt(mundo->locais[local]->herois); i++)
		mundo->herois[mundo->locais[local]->herois->v[i]]->experiencia += aleat(1, 5);

	return;
}

void tratar_evento_chegada(mundo_t *mundo, evento_t *evento){
	evento_t *aux;
	int heroi = evento->dado1;
	int local = evento->dado2;

	printf("%6d:CHEGA HEROI %2d Local %d ", mundo->tempoAtual, mundo->herois[heroi]->idHeroi, mundo->locais[local]->idLocal);

	/* se a cardinalidade do conjunto de herois do local for igual a lotacao maxima do mesmo, testa se ele vai ficar na fila. */
	if(cardinalidade_cjt(mundo->locais[local]->herois) == mundo->locais[local]->lotacaoMaxima){

		/* se o heroi for paciente, adiciona ele na fila. */
		if(mundo->herois[heroi]->paciencia/4 - tamanho_fila(mundo->locais[local]->fila) > 0){
			insere_fila(mundo->locais[local]->fila, mundo->herois[heroi]->idHeroi);

			printf("(%2d/%2d), FILA %2d\n", cardinalidade_cjt(mundo->locais[local]->herois), mundo->locais[local]->lotacaoMaxima, mundo->locais[local]->fila->tamanho);
			return;
		}

		/* se o heroi nao for paciente, cria um evento de saida do local atual. */ 
		printf("(%2d/%2d), DESISTE\n", cardinalidade_cjt(mundo->locais[local]->herois), mundo->locais[local]->lotacaoMaxima);

		aux = cria_evento(SAIDA, heroi, local, mundo->tempoAtual);
		adiciona_ordem_lef(mundo->lef, aux);
		free(aux);
		return;
	}

	/* se a cardinalidade do conjunto de herois do local for menor que a lotacao maxima, insere o heroi no conjunto de herois do local. */
	insere_cjt(mundo->locais[local]->herois, heroi); 

	printf("(%2d/%2d), ENTRA\n", cardinalidade_cjt(mundo->locais[local]->herois), mundo->locais[local]->lotacaoMaxima);

	/* cria um evento de saida do local atual para o heroi, calculando o tempo que ele vai passar no mesmo. */ 
	aux = cria_evento(SAIDA, heroi, local, mundo->tempoAtual + max(1, mundo->herois[heroi]->paciencia/10 + aleat(-2, 6)));
	adiciona_ordem_lef(mundo->lef, aux);
	free(aux);
	return;
}

void tratar_evento_saida(mundo_t *mundo, evento_t *evento){
	evento_t *aux;
	int heroi = evento->dado1;
	int local = evento->dado2;
	int heroi_fila, destino;

	/* retira o heroi do conjunto de herois do lugar atual. */
	retira_cjt(mundo->locais[local]->herois, mundo->herois[heroi]->idHeroi);
	printf("%6d:SAIDA HEROI %2d Local %d ", mundo->tempoAtual, mundo->herois[heroi]->idHeroi, mundo->locais[local]->idLocal);

	/* se a fila do local atual nao estiver vazia. */
	if(!vazia_fila(mundo->locais[local]->fila)){

		/* retira o primeiro heroi da fila e cria um evento de chegada para o mesmo no local atual. */
		retira_fila(mundo->locais[local]->fila, &heroi_fila);

		printf("(%2d/%2d), REMOVE FILA HEROI %2d\n", cardinalidade_cjt(mundo->locais[local]->herois), mundo->locais[local]->lotacaoMaxima, heroi_fila);

		aux = cria_evento(CHEGADA, heroi_fila, local, mundo->tempoAtual);
		adiciona_inicio_lef(mundo->lef, aux);
		free(aux);
		return;
	}
	printf("(%2d/%2d)\n", cardinalidade_cjt(mundo->locais[local]->herois), mundo->locais[local]->lotacaoMaxima);

	/* cria um evento de chegada do heroi para o lugar de destino, calculando o tempo de deslocamento entre os locais. */
	destino = aleat(0, N_LOCAIS - 1);
	aux = cria_evento(CHEGADA, heroi, destino, mundo->tempoAtual + tdl(mundo, heroi, local, destino) / 15);
	adiciona_ordem_lef(mundo->lef, aux);
	free(aux);
	return;
}

void tratar_evento_missao(mundo_t *mundo, evento_t *evento){
	evento_t *aux;
	conjunto_t *missao;
	int idMissao = evento->dado1;
	int local;

	missao = cria_subcjt_cjt(mundo->habilidades,aleat(3, 6));

	/* imprime as habilidades requeridas para realizar a missao. */
	printf("%6d:MISSAO %2d HAB_REQ ", mundo->tempoAtual, idMissao);
	imprime_cjt(missao);

	/* encontra o local onde sera realizada a missao, se nenhuma equipe conseguir realizar a missao, retorna -1. */
	local = id_local_equipe_escolhida(mundo, evento, missao);

	/* se encontrar o local onde a missao sera realizada. */
	if(local >= 0){
		printf("%6d:MISSAO %2d HAB_EQS %d:", mundo->tempoAtual, idMissao, local);

		imprime_cjt(mundo->locais[local]->herois);
		atualizar_xp(mundo, local);
	}

	/* se nenhuma equipe conseguir realizar a missao. */
	else{
		printf("%6d:MISSAO %2d IMPOSSIVEL\n", mundo->tempoAtual, idMissao);

		aux = cria_evento(MISSAO, idMissao, 0, aleat(mundo->tempoAtual, FIM_DO_MUNDO));
		adiciona_ordem_lef(mundo->lef, aux);
		free(aux);
	}
	destroi_cjt(missao);
	return;
}

/* desaloca toda a memoria alocada no mundo. */
mundo_t *destroi_mundo(mundo_t *mundo){
	destroi_cjt(mundo->habilidades);
	destroi_vetor_herois(mundo->herois);
	destroi_vetor_locais(mundo->locais);
	destroi_lef(mundo->lef);
	free(mundo);
	return NULL;
}

/* termina a simulacao. */
mundo_t *fim_do_mundo(mundo_t *mundo){
	int i;

	printf("%6d:FIM\n", mundo->tempoAtual);

	for(i = 0; i < N_HEROIS; i++)
		printf("HEROI %2d EXPERIENCIA %2d\n", mundo->herois[i]->idHeroi, mundo->herois[i]->experiencia);

	return destroi_mundo(mundo);
}
