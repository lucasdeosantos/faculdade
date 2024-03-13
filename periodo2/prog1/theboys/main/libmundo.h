#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "liblef.h"
#include "libfila.h"
#include "libconjunto.h"

#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES*5
#define N_LOCAIS N_HEROIS/6
#define N_MISSOES FIM_DO_MUNDO/100
#define FIM_DO_MUNDO 34944

#define CHEGADA 1
#define SAIDA 2
#define MISSAO 3
#define FIM 4

typedef struct heroi {
	int idHeroi;
	int paciencia;
	int idade;
	int experiencia;
	conjunto_t *habilidades;
} heroi_t;

typedef struct ponto {
	int x;
	int y;
} ponto_t;

typedef struct local {
	int idLocal;
	int lotacaoMaxima;
	conjunto_t *herois;
	fila_t *fila;
	ponto_t localizacao;
} local_t;

typedef struct mundo {
	int tempoAtual;
	conjunto_t *habilidades;
	heroi_t **herois;
	local_t **locais;
	lef_t *lef;
	int nHerois;
	int nLocais;
	int tamanhoMundo;
} mundo_t;

/* inicia a simulacao. */
mundo_t *cria_mundo();

/* trata dos eventos de chegada e imprime o resultado na tela. */
void tratar_evento_chegada(mundo_t *mundo, evento_t *evento);

/* trata dos eventos de saida e imprime o resultado na tela. */
void tratar_evento_saida(mundo_t *mundo, evento_t *evento);

/*trata dos eventos de missoes e imprime o resultado na tela. */
void tratar_evento_missao(mundo_t *mundo, evento_t*evento);

/* termina a simulacao. */
mundo_t *fim_do_mundo(mundo_t *mundo);
