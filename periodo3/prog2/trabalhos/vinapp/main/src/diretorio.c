#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include "diretorio.h"

struct arquivo *iniciarArquivo(char *nome, off_t posicao)
{
	struct arquivo *arq = malloc(sizeof(struct arquivo));

	if (!arq) {
		fprintf(stderr, "Erro ao alocar arquivo.\n");
		return NULL;
	}

	arq->nome = malloc((strlen(nome) + 2) * sizeof(char));
	if (!arq->nome) {
		fprintf(stderr, "Erro ao alocar o nome do arquivo.\n");
		free(arq);
		return NULL;
	}

	strcpy(arq->nome, nome);
	arq->posicao = posicao;

	struct stat info;
	if (stat(arq->nome, &info)) {
		fprintf(stderr, "Erro ao obter as informacoes do arquivo.\n");
		liberarArquivo(arq);
		return NULL;
	}

	arq->tamanho = info.st_size;
	arq->permissoes = info.st_mode;
	arq->usuario = info.st_uid;
	arq->grupo = info.st_gid;
	arq->dataAcesso = info.st_atime;
	arq->dataModificacao = info.st_mtime;

	return arq;
}

struct nodoArquivo *iniciarNodoArquivo(struct arquivo *arq)
{
	struct nodoArquivo *nodo = malloc(sizeof(struct nodoArquivo));

	if (!nodo) {
		fprintf(stderr, "Erro ao alocar nodo.\n");
		return NULL;
	}

	nodo->arq = arq;
	nodo->antr = NULL;
	nodo->prox = NULL;

	return nodo;
}

struct diretorio *iniciarDiretorio()
{
	struct diretorio *dir = calloc(1, sizeof(struct diretorio));

	return dir;
}

int inserirArquivoDiretorio(struct diretorio *dir, struct arquivo *arq)
{	
	struct nodoArquivo *aux = dir->ini;
	struct nodoArquivo *nodo = iniciarNodoArquivo(arq);

	if (!nodo) {
		fprintf(stderr, "Erro na inicializacao do nodo.\n");
		return 0;
	}

	if (!aux) {
		dir->ini = nodo;
		dir->numArquivos++;
		return 1;
	}

	while (aux->prox)
		aux = aux->prox;

	aux->prox = nodo;
	nodo->antr = aux;
	dir->numArquivos++;

	return 1;
}

struct nodoArquivo *buscarNodoArquivo(struct diretorio *dir, char *nomeArquivo)
{
	for(struct nodoArquivo *nodo = dir->ini; nodo; nodo = nodo->prox) {
		if (nomeArquivo[0] == '/') {
			size_t tamanho = strlen(nomeArquivo) + 2;

			char *tempNome = malloc(tamanho * sizeof(char));
			if (!tempNome) {
				fprintf(stderr, "Erro na alocacao do nome temporario.");
				return NULL;
			}

			snprintf(tempNome, tamanho, ".%s", nomeArquivo);
			if (!strcmp(nodo->arq->nome, tempNome)) {
				free(tempNome);
				return nodo;
			}
			free(tempNome);
		}
		else {
			if (!strcmp(nodo->arq->nome, nomeArquivo))
				return nodo;
		}
	}

	return NULL;
}

void removerArquivoDiretorio(struct diretorio *dir, struct nodoArquivo *nodo)
{
	if (!nodo->antr && !nodo->prox) {
		dir->ini = NULL;
	}
	else if (!nodo->antr) {
		nodo->prox->antr = nodo->antr;
		dir->ini = nodo->prox;
	}
	else {
		if (nodo->antr)
			nodo->antr->prox = nodo->prox;
		if (nodo->prox)
			nodo->prox->antr = nodo->antr;
	}

	liberarArquivo(nodo->arq);	
	free(nodo);
	dir->numArquivos--;
}

void moverArquivosDiretorio(struct diretorio *dir, struct nodoArquivo *target, struct nodoArquivo *nodo)
{
	if (nodo == dir->ini) {
		dir->ini = nodo->prox;
		dir->ini->antr = nodo->antr;
	}

	if (nodo->antr)
		nodo->antr->prox = nodo->prox;

	if (nodo->prox)
		nodo->prox->antr = nodo->antr;

	if (target->prox)
		target->prox->antr = nodo;

	nodo->prox = target->prox;
	nodo->antr = target;
	target->prox = nodo;
}

struct arquivo *lerArquivo(FILE *vpp)
{
	struct arquivo *arq = malloc(sizeof(struct arquivo));
	if (!arq) {
		fprintf(stderr, "Erro ao alocar arquivo.\n");
		return NULL;
	}

	size_t tamanhoNome;
	if (!fread(&tamanhoNome, sizeof(size_t), 1, vpp)) {
		fprintf(stderr, "Erro ao ler o tamanho do nome do arquivo.\n");
		free(arq);
		return NULL;
	}
	
	arq->nome = malloc((tamanhoNome + 1) * sizeof(char));
	if (!arq->nome) {
		fprintf(stderr, "Erro ao alocar o nome do arquivo.\n");
		free(arq);
		return NULL;
	}
	
	if (!fread(arq->nome, sizeof(char), tamanhoNome, vpp)) {
		fprintf(stderr, "Erro ao ler o nome do arquivo.\n");
		liberarArquivo(arq);
		return NULL;
	}
	arq->nome[tamanhoNome] = '\0';

	if (!fread(&arq->posicao, sizeof(off_t), 1, vpp)) {
		fprintf(stderr, "Erro ao ler a posicao do arquivo.\n");
		liberarArquivo(arq);
		return NULL;
	}

	if (!fread(&arq->tamanho, sizeof(off_t), 1, vpp)) {
		fprintf(stderr, "Erro ao ler o tamanho do arquivo.\n");
		liberarArquivo(arq);
		return NULL;
	}

	if (!fread(&arq->permissoes, sizeof(mode_t), 1, vpp)) {
		fprintf(stderr, "Erro ao ler as permissoes do arquivo.\n");
		liberarArquivo(arq);
		return NULL;
	}

	if (!fread(&arq->usuario, sizeof(uid_t), 1, vpp)) {
		fprintf(stderr, "Erro ao ler o usuario do arquivo.\n");
		liberarArquivo(arq);
		return NULL;
	}

	if (!fread(&arq->grupo, sizeof(gid_t), 1, vpp)) {
		fprintf(stderr, "Erro ao ler o grupo do arquivo.\n");
		liberarArquivo(arq);
		return NULL;
	}

	if (!fread(&arq->dataAcesso, sizeof(time_t), 1, vpp)) {
		fprintf(stderr, "Erro ao ler a data de acesso do arquivo.\n");
		liberarArquivo(arq);
		return NULL;
	}

	if (!fread(&arq->dataModificacao, sizeof(time_t), 1, vpp)) {
		fprintf(stderr, "Erro ao ler a data de modificacao do arquivo.\n");
		liberarArquivo(arq);
		return NULL;
	}

	return arq;
}

struct diretorio *lerDiretorio(FILE *vpp)
{
	off_t posicao;
	rewind(vpp);

	if (!fread(&posicao, sizeof(off_t), 1, vpp)) {
		if (feof(vpp)) {
			struct diretorio *dir = iniciarDiretorio();
			if (!dir) {
				fprintf(stderr, "Erro na inicializacao do diretorio.\n");
				return NULL;
			}

			posicao = sizeof(off_t);
			rewind(vpp);
			fwrite(&posicao, sizeof(off_t), 1, vpp);

			return dir;
		}
		else {
			fprintf(stderr, "Erro na leitura da posicao do diretorio.\n");
			return NULL;
		}
	}

	fseeko(vpp, posicao, SEEK_SET);

	struct diretorio *dir = malloc(sizeof(struct diretorio));
	if (!dir) {
		fprintf(stderr, "Erro ao alocar diretorio.\n");
		return NULL;
	}

	if (!fread(&(dir->numArquivos), sizeof(size_t), 1, vpp)) {
		fprintf(stderr, "Erro ao ler numero de arquivos do diretorio.\n");
		free(dir);
		return NULL;
	}

	dir->ini = NULL;
	struct nodoArquivo *nodoAnterior = NULL;

	for (size_t i = 0; i < dir->numArquivos; i++) {
		struct arquivo *arq = lerArquivo(vpp);	
		if (!arq) {
			fprintf(stderr, "Erro na leitura do arquivo.\n");
			liberarDiretorio(dir);
			return NULL;
		}

		struct nodoArquivo *nodo = iniciarNodoArquivo(arq);
		if (!nodo) {
			fprintf(stderr, "Erro na inicializacao do nodo.\n");
			liberarArquivo(arq);
			liberarDiretorio(dir);
			return NULL;
		}

		if (!dir->ini) {
			dir->ini = nodo;
		}
		else {
			nodoAnterior->prox = nodo;
			nodo->antr = nodoAnterior;
		}
		nodoAnterior = nodo;
	}
	return dir;
}

void escreverArquivo(FILE *vpp, struct arquivo *arq)
{
	size_t tamanhoNome = strlen(arq->nome);
	fwrite(&tamanhoNome, sizeof(size_t), 1, vpp);
	fwrite(arq->nome, sizeof(char), tamanhoNome, vpp);
	fwrite(&(arq->posicao), sizeof(off_t), 1, vpp);
	fwrite(&(arq->tamanho), sizeof(off_t), 1, vpp);
	fwrite(&(arq->permissoes), sizeof(mode_t), 1, vpp);
	fwrite(&(arq->usuario), sizeof(uid_t), 1, vpp);
	fwrite(&(arq->grupo), sizeof(gid_t), 1, vpp);
	fwrite(&(arq->dataAcesso), sizeof(time_t), 1, vpp);
	fwrite(&(arq->dataModificacao), sizeof(time_t), 1, vpp);
}

void escreverDiretorio(FILE *vpp, struct diretorio *dir)
{
	off_t posicao;
	rewind(vpp);
	fread(&posicao, sizeof(off_t), 1, vpp);

	fseeko(vpp, posicao, SEEK_SET);
	fwrite(&(dir->numArquivos), sizeof(size_t), 1, vpp);

	for (struct nodoArquivo *nodo = dir->ini; nodo; nodo = nodo->prox)
		escreverArquivo(vpp, nodo->arq);
}

void imprimirDiretorio(struct diretorio *dir)
{
	size_t ordem = 1;
	for (struct nodoArquivo *nodo = dir->ini; nodo; nodo = nodo->prox, ordem++) {
		unsigned int permissoes = nodo->arq->permissoes & 0777;

		printf("%c", '-');

		printf("%c", (permissoes & S_IRUSR) ? 'r' : '-');
		printf("%c", (permissoes & S_IWUSR) ? 'w' : '-');
		printf("%c", (permissoes & S_IXUSR) ? 'x' : '-');

		printf("%c", (permissoes & S_IRGRP) ? 'r' : '-');
		printf("%c", (permissoes & S_IWGRP) ? 'w' : '-');
		printf("%c", (permissoes & S_IXGRP) ? 'x' : '-');

		printf("%c", (permissoes & S_IROTH) ? 'r' : '-');
		printf("%c", (permissoes & S_IWOTH) ? 'w' : '-');
		printf("%c", (permissoes & S_IXOTH) ? 'x' : '-');

		struct passwd *pwd = getpwuid(nodo->arq->usuario);
		if (pwd)
			printf(" %s", pwd->pw_name);
		else
			printf(" %u", nodo->arq->usuario);

		printf("%c", '/');

		struct group *gp = getgrgid(nodo->arq->grupo);
		if (gp)
			printf("%s ", gp->gr_name);
		else
			printf("%u ", nodo->arq->grupo);

		printf("%lld ", (long long)nodo->arq->tamanho);

		char *dataModificacao = ctime(&(nodo->arq->dataModificacao));
		size_t tamanho = strlen(dataModificacao);
		dataModificacao[tamanho - 1] = '\0';

		printf("%s ", dataModificacao);
		printf("%zu ", ordem);
		printf("%s\n", nodo->arq->nome);
	}
}

void liberarArquivo(struct arquivo *arq)
{
	if (!arq)
		return;

	free(arq->nome);
	free(arq);
}

void liberarDiretorio(struct diretorio *dir)
{
	while (dir->ini) {
		struct nodoArquivo *nodo = dir->ini;
		dir->ini = dir->ini->prox;
		liberarArquivo(nodo->arq);
		free(nodo);
	} 
	free(dir);
}
