#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabelaHash.h"

void matarProgramaFaltaMemoria()
{
	fputs("Falha ao alocar memoria.\n", stderr);
	exit(1);
}

struct aluno *getAluno1()
{
	struct aluno *retorno = malloc(sizeof(struct aluno));

	if (!retorno) matarProgramaFaltaMemoria();
	retorno->nome = malloc(sizeof("Lucas Emanuel de Oliveira Santos"));

	if (!retorno->nome)
		matarProgramaFaltaMemoria();

	retorno->nomeDinf = malloc(sizeof("leos22"));
	if (!(retorno->nomeDinf))
		matarProgramaFaltaMemoria();

	strcpy(retorno->nome, "Lucas Emanuel de Oliveira Santos");
	strcpy(retorno->nomeDinf, "leos22");
	retorno->grr = 20224379;

	return retorno;
}

struct aluno *getAluno2()
{
	return NULL;
}

void imprimirDadosAlunos()
{
	struct aluno *aluno = getAluno1();

	printf("Trabalho de %s\n", aluno->nome);
	printf("Login Dinf %s\n", aluno->nomeDinf);
	printf("GRR %u\n", aluno->grr);

	free(aluno->nome);
	free(aluno->nomeDinf);
	free(aluno);

	aluno = getAluno2();
	if (!aluno) return;

	printf("...E... \n\n");
	printf("Trabalho de %s\n", aluno->nome);
	printf("Login Dinf %s\n", aluno->nomeDinf);
	printf("GRR %u\n\n", aluno->grr);

	free(aluno->nome);
	free(aluno->nomeDinf);
	free(aluno);
}

struct nodoHash **gerarTabelaHash(size_t tamTabela)
{
	struct nodoHash **tabelaHash = calloc(tamTabela, sizeof(struct nodoHash*));

	return tabelaHash;
}

size_t hash(int chave, size_t tamTabela)
{
	return (unsigned int)chave % tamTabela;
}

struct nodoHash *criaNodoHash(int chave)
{
	struct nodoHash *nodo = malloc(sizeof(struct nodoHash));

	if (!nodo)
		return NULL;

	nodo->chave = chave;
	nodo->anterior = NULL;
	nodo->proximo = NULL;

	return nodo;
}

struct nodoHash *inserir(struct nodoHash *tabelaHash[], size_t tamTabela, int chave)
{
	size_t i = hash(chave, tamTabela);
	struct nodoHash *aux = tabelaHash[i];
	struct nodoHash *anterior;

	if (!aux) { // lista vazia.
		tabelaHash[i] = criaNodoHash(chave);
		return tabelaHash[i];
	}

	while (aux) { // adiciona no final da lista.
		if (aux->chave == chave) // testa se é duplicada.
			return NULL;

		anterior = aux;
		aux = aux->proximo;
	}
	
	struct nodoHash *nodo = criaNodoHash(chave);
	if (!nodo)
		return NULL;

	anterior->proximo = nodo;
	nodo->anterior = anterior;

	return nodo;
}

void liberarNodosHash(struct nodoHash *nodo)
{
	while (nodo) {
		struct nodoHash *aux = nodo;
		nodo = nodo->proximo;
		free(aux);
	}
}

void liberarTabelaHash(struct nodoHash *tabelaHash[], size_t tamTabela)
{
	for(size_t i = 0; i < tamTabela; i++)
		if (tabelaHash[i])
			liberarNodosHash(tabelaHash[i]);

	free(tabelaHash);
}

void imprimirTabelaHash(struct nodoHash *tabelaHash[], size_t tamTabela)
{
	for (size_t i = 0; i < tamTabela; i++) {
		printf("%zu ", i);
		struct nodoHash *nodo = tabelaHash[i];
		while (nodo) {
			printf("[%d] -> ", nodo->chave);
			nodo = nodo->proximo;
		}
		printf("NULL\n");
	}
}

struct nodoHash *buscar(struct nodoHash *tabelaHash[], size_t tamTabela, int chave)
{
	size_t i = hash(chave, tamTabela);

	if (!tabelaHash[i]) // lista vazia.
		return NULL;

	struct nodoHash *nodo = tabelaHash[i];

	while (nodo) {
		if (nodo->chave == chave)
			return nodo;

		nodo = nodo->proximo;
	}
	return NULL;
}

void excluir(struct nodoHash *tabelaHash[], size_t tamTabela, struct nodoHash *nodo)
{
	size_t i = hash(nodo->chave, tamTabela);

	if (!nodo->anterior && !nodo->proximo) { // unico elemento da lista.
		tabelaHash[i] = NULL;
	}
	else if (!nodo->anterior) { // primeiro elemento da lista.
		nodo->proximo->anterior = nodo->anterior;
		tabelaHash[i] = nodo->proximo;
	}
	else {
		if (nodo->anterior)
			nodo->anterior->proximo = nodo->proximo;
		if (nodo->proximo)
			nodo->proximo->anterior = nodo->anterior;
	}
	free(nodo);
}
