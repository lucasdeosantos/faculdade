#ifndef DIRETORIO_H
#define DIRETORIO_H

#include <stdio.h>
#include <sys/types.h>

struct arquivo {
	char *nome;
	off_t posicao;
	off_t tamanho;
	mode_t permissoes;
	uid_t usuario;
	gid_t grupo;
	time_t dataAcesso;
	time_t dataModificacao;
};

struct nodoArquivo {
	struct arquivo *arq;
	struct nodoArquivo *antr;
	struct nodoArquivo *prox;
};

struct diretorio {
	size_t numArquivos;
	struct nodoArquivo *ini;
};

struct arquivo *iniciarArquivo(char *nome, off_t posicao);

struct diretorio *iniciarDiretorio();

int inserirArquivoDiretorio(struct diretorio *dir, struct arquivo *arq);

struct nodoArquivo *buscarNodoArquivo(struct diretorio *dir, char *nomeArquivo);

void removerArquivoDiretorio(struct diretorio *dir, struct nodoArquivo *nodo);

void moverArquivosDiretorio(struct diretorio *dir, struct nodoArquivo *target, struct nodoArquivo *nodo);

struct diretorio *lerDiretorio(FILE *vpp);

void escreverDiretorio(FILE *vpp, struct diretorio *dir);

void imprimirDiretorio(struct diretorio *dir);

void liberarArquivo(struct arquivo *arq);

void liberarDiretorio(struct diretorio *dir);

#endif // DIRETORIO_H
