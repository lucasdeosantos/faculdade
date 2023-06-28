#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <utime.h>
#include <libgen.h> 
#include <limits.h>
#include "vinapp.h"

int copiarArquivo(FILE *origem, FILE *destino, lldiv_t blocos)
{
	for (int i = 0; i < blocos.quot; i++) {
		char *buffer = malloc(BUFFER_SIZE * sizeof(char));
		if (!buffer) {
			fprintf(stderr, "Erro ao alocar memoria para o buffer.\n");
			return 0;
		}

		fread(buffer, sizeof(char), BUFFER_SIZE, origem);
		fwrite(buffer, sizeof(char), BUFFER_SIZE, destino);
		free(buffer);
	}

	if (blocos.rem) {
		char *buffer = malloc(blocos.rem * sizeof(char));
		if (!buffer) {
			fprintf(stderr, "Erro ao alocar memoria para o buffer.\n");
			return 0;
		}

		fread(buffer, sizeof(char), blocos.rem, origem);
		fwrite(buffer, sizeof(char), blocos.rem, destino);
		free(buffer);
	}
	
	return 1;
}

int moverEsquerda(FILE *vpp, off_t posicaoEscrita, off_t posicaoLeitura)
{
	fseeko(vpp, 0, SEEK_END);
	off_t final = ftello(vpp);
	off_t deslocamento = final - posicaoLeitura;
	final -= (posicaoLeitura - posicaoEscrita);

	lldiv_t blocos = lldiv(deslocamento, BUFFER_SIZE);

	for (int i = 0; i < blocos.quot; i++) {
		char *buffer = malloc(BUFFER_SIZE * sizeof(char));
		if (!buffer) {
			fprintf(stderr, "Erro ao alocar memoria para o buffer.\n");
			return 0;
		}

		fseeko(vpp, posicaoLeitura, SEEK_SET);
		fread(buffer, sizeof(char), BUFFER_SIZE, vpp);
		posicaoLeitura += BUFFER_SIZE;

		fseeko(vpp, posicaoEscrita, SEEK_SET);
		fwrite(buffer, sizeof(char), BUFFER_SIZE, vpp);
		posicaoEscrita += BUFFER_SIZE;

		free(buffer);
	}

	if (blocos.rem) {
		char *buffer = malloc(blocos.rem * sizeof(char));
		if (!buffer) {
			fprintf(stderr, "Erro ao alocar memoria para o buffer.\n");
			return 0;
		}

		fseeko(vpp, posicaoLeitura, SEEK_SET);
		fread(buffer, sizeof(char), blocos.rem, vpp);

		fseeko(vpp, posicaoEscrita, SEEK_SET);
		fwrite(buffer, sizeof(char), blocos.rem, vpp);

		free(buffer);
	}

	int trunc = ftruncate(fileno(vpp), final);
	if (trunc == -1) {
		fprintf(stderr, "Erro ao redimensionar arquivo.\n");
		return 0;
	}

	return 1;
}

int moverDireita(FILE *vpp, off_t inicioDeslocamento, off_t deslocamento)
{
	fseeko(vpp, 0, SEEK_END);
	off_t posicaoLeitura = ftello(vpp);
	off_t final = posicaoLeitura + deslocamento;

	int trunc = ftruncate(fileno(vpp), final);
	if (trunc == -1) {
		fprintf(stderr, "Erro ao redimensionar arquivo.\n");
		return 0;
	}

	fseeko(vpp, 0, SEEK_END);
	off_t posicaoEscrita = ftello(vpp);

	lldiv_t blocos = lldiv(posicaoLeitura - inicioDeslocamento, BUFFER_SIZE);

	for (int i = 0; i < blocos.quot; i++) {
		char *buffer = malloc(BUFFER_SIZE * sizeof(char));
		if (!buffer) {
			fprintf(stderr, "Erro ao alocar memoria para o buffer.\n");
			return 0;
		}

		posicaoLeitura -= BUFFER_SIZE;
		fseeko(vpp, posicaoLeitura, SEEK_SET);
		fread(buffer, sizeof(char), BUFFER_SIZE, vpp);

		posicaoEscrita -= BUFFER_SIZE;
		fseeko(vpp, posicaoEscrita, SEEK_SET);
		fwrite(buffer, sizeof(char), BUFFER_SIZE, vpp);

		free(buffer);
	}

	if (blocos.rem) {
		char *buffer = malloc(blocos.rem * sizeof(char));
		if (!buffer) {
			fprintf(stderr, "Erro ao alocar memoria para o buffer.\n");
			return 0;
		}

		posicaoLeitura -= blocos.rem;
		fseeko(vpp, posicaoLeitura, SEEK_SET);
		fread(buffer, sizeof(char), blocos.rem, vpp);

		posicaoEscrita -= blocos.rem;
		fseeko(vpp, posicaoEscrita, SEEK_SET);
		fwrite(buffer, sizeof(char), blocos.rem, vpp);

		free(buffer);
	}

	return 1;
}

void corrigirPosicao(FILE *vpp, struct diretorio *dir)
{
	if (dir->ini) {
		dir->ini->arq->posicao = sizeof(off_t);

		off_t posicaoProx = dir->ini->arq->posicao + dir->ini->arq->tamanho;
		struct nodoArquivo *nodo = dir->ini->prox;

		while (nodo) {
			nodo->arq->posicao = posicaoProx;
			posicaoProx = nodo->arq->posicao + nodo->arq->tamanho;
			nodo = nodo->prox;
		}

		rewind(vpp);
		fwrite(&posicaoProx, 1, sizeof(off_t), vpp);
	}
	else {
		off_t posicao = sizeof(off_t);
		rewind(vpp);
		fwrite(&posicao, 1, sizeof(off_t), vpp);
	}
}

void corrigirCaminho(char *caminho)
{
	if (caminho[0] == '/') {
		memmove(caminho + 1, caminho, strlen(caminho) + 1);
		caminho[0] = '.';
	}
}

int acrescentar(FILE *vpp, struct diretorio *dir, char *arquivo)
{
	off_t posicao;
	rewind(vpp);
	fread(&posicao, 1, sizeof(off_t), vpp);

	int trunc = ftruncate(fileno(vpp), posicao);
	if (trunc == -1) {
		fprintf(stderr, "Erro ao redimensionar arquivo.\n");
		return 0;
	}

	struct arquivo *arq = iniciarArquivo(arquivo, posicao);
	if (!arq) {
		fprintf(stderr, "Erro na inicializacao do arquivo.\n");
		return 0;
	}

	FILE *novo = fopen(arquivo, "r");
	if (!novo) {
		fprintf(stderr, "Erro ao abrir arquivo.\n");
		liberarArquivo(arq);
		return 0;
	}

	lldiv_t blocos = lldiv(arq->tamanho, BUFFER_SIZE);

	fseeko(vpp, posicao, SEEK_SET);
	if (!copiarArquivo(novo, vpp, blocos)) {
		fprintf(stderr, "Erro ao copiar arquivo.\n");
		liberarArquivo(arq);
		fclose(novo);
		return 0;
	}

	corrigirCaminho(arq->nome);
	inserirArquivoDiretorio(dir, arq);

	fseeko(vpp, 0, SEEK_END);
	posicao = ftello(vpp);
	rewind(vpp);
	fwrite(&posicao, 1, sizeof(off_t), vpp);

	fclose(novo);
	return 1;
}

int substituir(FILE *vpp, struct diretorio *dir, struct nodoArquivo *nodo, struct arquivo *novo)
{
	struct arquivo *antigo = nodo->arq;

	off_t posicao;
	rewind(vpp);
	fread(&posicao, 1, sizeof(off_t), vpp);

	int trunc = ftruncate(fileno(vpp), posicao);
	if (trunc == -1) {
		fprintf(stderr, "Erro ao redimensionar arquivo.\n");
		return 0;
	}

	FILE *arquivo = fopen(novo->nome, "r");
	if (!arquivo) {
		fprintf(stderr, "Erro ao abrir arquivo.\n");
		return 0;
	}

	lldiv_t blocos = lldiv(novo->tamanho, BUFFER_SIZE);

	if (nodo->arq->tamanho == novo->tamanho) {
		fseeko(vpp, nodo->arq->posicao, SEEK_SET);
		if (!copiarArquivo(arquivo, vpp, blocos)) {
			fprintf(stderr, "Erro ao copiar arquivo.\n");
			fclose(arquivo);
			return 0;
		}
		nodo->arq = novo;
	}
	else {
		if (nodo->arq->tamanho > novo->tamanho) {
			fseeko(vpp, nodo->arq->posicao, SEEK_SET);
			if (!copiarArquivo(arquivo, vpp, blocos)) {
				fprintf(stderr, "Erro ao copiar arquivo.\n");
				fclose(arquivo);
				return 0;
			}

			off_t posicaoEscrita = nodo->arq->posicao + novo->tamanho;
			off_t posicaoLeitura = nodo->arq->posicao + nodo->arq->tamanho;

			if (!moverEsquerda(vpp, posicaoEscrita, posicaoLeitura)) {
				fprintf(stderr, "Erro ao movimentar dados no arquivo.\n");
				fclose(arquivo);
				return 0;
			}
		}
		else {
			off_t inicioDeslocamento = nodo->arq->posicao + nodo->arq->tamanho;
			off_t deslocamento = novo->tamanho - nodo->arq->tamanho;

			if (!moverDireita(vpp, inicioDeslocamento, deslocamento)) {
				fprintf(stderr, "Erro ao movimentar dados no arquivo.\n");
				fclose(arquivo);
				return 0;
			}

			fseeko(vpp, nodo->arq->posicao, SEEK_SET);
			if (!copiarArquivo(arquivo, vpp, blocos)) {
				fprintf(stderr, "Erro ao copiar arquivo.\n");
				fclose(arquivo);
				return 0;
			}
		}
		nodo->arq = novo;
		corrigirPosicao(vpp, dir);
	}

	liberarArquivo(antigo);
	fclose(arquivo);
	return 1;
}

int inserirSempre(FILE *vpp, struct diretorio *dir, char *arquivo)
{
	struct nodoArquivo *nodo = buscarNodoArquivo(dir, arquivo);
	if (nodo) {
		struct arquivo *arq = iniciarArquivo(arquivo, nodo->arq->posicao);
		if (!arq) {
			fprintf(stderr, "Erro na inicializacao do arquivo.\n");
			return 0;
		}
		if (!substituir(vpp, dir, nodo, arq)) {
			liberarArquivo(arq);
			return 0;
		}
		return 1;
	}

	if (!acrescentar(vpp, dir, arquivo))
		return 0;

	return 1;
}

int inserirMaisRecente(FILE *vpp, struct diretorio *dir, char *arquivo)
{
	struct nodoArquivo *nodo = buscarNodoArquivo(dir, arquivo);
	if (nodo) {
		struct arquivo *arq = iniciarArquivo(arquivo, nodo->arq->posicao);
		if (!arq) {
			fprintf(stderr, "Erro na inicializacao do arquivo.\n");
			return 0;
		}

		time_t diff = difftime(arq->dataModificacao, nodo->arq->dataModificacao);

		if (diff > 0) {
			if (!substituir(vpp, dir, nodo, arq)) {
				liberarArquivo(arq);
				return 0;
			}
			return 1;
		}

		liberarArquivo(arq);
		return 0;
	}

	if (!acrescentar(vpp, dir, arquivo))
		return 0;

	return 1;
}

int inserir(int (*funcao) (FILE *, struct diretorio *, char *), FILE *vpp, struct diretorio *dir, char *arquivo)
{
	if (!funcao(vpp, dir, arquivo))
		return 0;

	return 1;
}

int remover(FILE *vpp, struct diretorio *dir, char *arquivo)
{
	struct nodoArquivo *nodo = buscarNodoArquivo(dir, arquivo);
	if (!nodo) {
		fprintf(stderr, "Erro: arquivo nao encontrado.\n");
		return 0;
	}

	off_t posicao;
	rewind(vpp);
	fread(&posicao, 1, sizeof(off_t), vpp);

	int trunc = ftruncate(fileno(vpp), posicao);
	if (trunc == -1) {
		fprintf(stderr, "Erro ao redimensionar arquivo.\n");
		return 0;
	}

	off_t posicaoEscrita = nodo->arq->posicao;
	off_t posicaoLeitura = nodo->arq->posicao + nodo->arq->tamanho;

	if (!moverEsquerda(vpp, posicaoEscrita, posicaoLeitura)) {
		fprintf(stderr, "Erro ao movimentar dados no arquivo.\n");
		return 0;
	}

	removerArquivoDiretorio(dir, nodo);
	corrigirPosicao(vpp, dir);

	return 1;
}

int mover(FILE *vpp, struct diretorio *dir, char *target, char *arquivo)
{
	struct nodoArquivo *nodoTarget = buscarNodoArquivo(dir, target);
	struct nodoArquivo *nodo = buscarNodoArquivo(dir, arquivo);

	if (!nodoTarget || !nodo) {
		fprintf(stderr, "Erro: arquivo nao encontrado.\n");
		return 0;
	}
		
	if (nodoTarget == nodo || nodoTarget->prox == nodo)
		return 1;
		
	off_t posicao;
	rewind(vpp);
	fread(&posicao, 1, sizeof(off_t), vpp);

	int trunc = ftruncate(fileno(vpp), posicao);
	if (trunc == -1) {
		fprintf(stderr, "Erro ao redimensionar arquivo.\n");
		return 0;
	}

	off_t inicioDeslocamento = nodoTarget->arq->posicao + nodoTarget->arq->tamanho;
	off_t deslocamento = nodo->arq->tamanho;

	if (!moverDireita(vpp, inicioDeslocamento, deslocamento)) {
		fprintf(stderr, "Erro ao movimentar dados no arquivo.\n");
		return 0;
	}

	off_t posicaoEscrita = inicioDeslocamento;
	off_t posicaoLeitura;

	if (nodoTarget->arq->posicao < nodo->arq->posicao)
		posicaoLeitura = nodo->arq->posicao + deslocamento;
	else
		posicaoLeitura = nodo->arq->posicao;

	lldiv_t blocos = lldiv(deslocamento, BUFFER_SIZE);

	for (int i = 0; i < blocos.quot; i++) {
		char *buffer = malloc(BUFFER_SIZE * sizeof(char));
		if (!buffer) {
			fprintf(stderr, "Erro ao alocar memoria para o buffer.\n");
			return 0;
		}

		fseeko(vpp, posicaoLeitura, SEEK_SET);
		fread(buffer, sizeof(char), BUFFER_SIZE, vpp);
		posicaoLeitura += BUFFER_SIZE;

		fseeko(vpp, posicaoEscrita, SEEK_SET);
		fwrite(buffer, sizeof(char), BUFFER_SIZE, vpp);
		posicaoEscrita += BUFFER_SIZE;

		free(buffer);
	}

	if (blocos.rem) {
		char *buffer = malloc(blocos.rem * sizeof(char));
		if (!buffer) {
			fprintf(stderr, "Erro ao alocar memoria para o buffer.\n");
			return 0;
		}

		fseeko(vpp, posicaoLeitura, SEEK_SET);
		fread(buffer, sizeof(char), blocos.rem, vpp);

		fseeko(vpp, posicaoEscrita, SEEK_SET);
		fwrite(buffer, sizeof(char), blocos.rem, vpp);

		free(buffer);
	}

	if (nodoTarget->arq->posicao < nodo->arq->posicao) {
		posicaoLeitura = nodo->arq->posicao + nodo->arq->tamanho + deslocamento;
		posicaoEscrita = nodo->arq->posicao + deslocamento;
	}
	else {
		posicaoLeitura = nodo->arq->posicao + nodo->arq->tamanho;
		posicaoEscrita = nodo->arq->posicao;
	}

	if (!moverEsquerda(vpp, posicaoEscrita, posicaoLeitura)) {
		fprintf(stderr, "Erro ao movimentar dados no arquivo.\n");
		return 0;
	}

	moverArquivosDiretorio(dir, nodoTarget, nodo);	
	corrigirPosicao(vpp, dir);

	return 1;
}

int criarCaminho(const char *arquivo)
{
	char *caminho = strdup(arquivo);
	char *diretorio = dirname(caminho);
	char *token = strtok(diretorio, "/");
	char path[PATH_MAX] = "";

	while (token != NULL) {
		strcat(path, token);
		strcat(path, "/");

		struct stat st;
		if (stat(path, &st) == -1) {
			if (mkdir(path, 0755) != 0) {
				fprintf(stderr, "Erro ao criar diretório.\n");
				free(caminho);
				return 0;
			}
		}
		token = strtok(NULL, "/");
	}

	free(caminho);
	return 1;
}

int extrair(FILE *vpp, struct diretorio *dir, char *arquivo)
{
	struct nodoArquivo *nodo = buscarNodoArquivo(dir, arquivo);
	if (!nodo) {
		fprintf(stderr, "Erro: arquivo nao encontrado.\n");
		return 0;
	}

	if (!criarCaminho(nodo->arq->nome)) {
		fprintf(stderr, "Erro ao criar caminho do arquivo.\n");
		return 0;
	}

	FILE *arq = fopen(nodo->arq->nome, "w");
	if (!arq) {
		fprintf(stderr, "Erro ao abrir arquivo.\n");
		return 0;
	}

	if (chmod(nodo->arq->nome, nodo->arq->permissoes) == -1) {
		fprintf(stderr, "Erro ao alterar as permissoes do arquivo.\n");
		fclose(arq);
		return 0;
	}

	if (chown(nodo->arq->nome, nodo->arq->usuario, nodo->arq->grupo) == -1) {
		fprintf(stderr, "Erro ao alterar o proprietario do arquivo.\n");
		fclose(arq);
		return 0;
	}

	struct utimbuf data;
	data.actime = nodo->arq->dataAcesso;
	data.modtime = nodo->arq->dataModificacao;

	if (utime(nodo->arq->nome, &data) == -1) {
		fprintf(stderr, "Erro ao alterar a data do arquivo.\n");
		fclose(arq);
		return 0;
	}

	lldiv_t blocos = lldiv(nodo->arq->tamanho, BUFFER_SIZE);

	fseeko(vpp, nodo->arq->posicao, SEEK_SET);
	if (!copiarArquivo(vpp, arq, blocos)) {
		fprintf(stderr, "Erro ao copiar arquivo.\n");
		fclose(arq);
		return 0;
	}

	fclose(arq);
	return 1;
}

void imprimirMensagemAjuda() {
	printf("Uso: vina++ <opção> <archive> [membro1 membro2 ...]\n\n");
	printf("-i: insere/acrescenta os membros ao archive. Caso o membro já exista no archive, ele é substituído.\n");
	printf("-a: mesmo comportamento da opção -i, mas a substituição de um membro existente ocorre APENAS caso o parâmetro seja mais recente que o arquivado.\n");
	printf("-m target: move o membro indicado para imediatamente depois do membro target existente em archive.\n");
	printf("-x: extrai os membros indicados de archive. Se os membros não forem indicados, todos são extraídos.\n");
	printf("-r: remove os membros indicados de archive.\n");
	printf("-c: lista o conteúdo de archive em ordem.\n");
	printf("-h: imprime uma mensagem de ajuda.\n");
}
