#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef AVL_H_
#define AVL_H_

struct nodo {
	int chave;
	struct nodo *fe;
	struct nodo *fd;
	struct nodo *pai;
	int h; // altura
};

struct aluno{
	char *nome;
	char *nomeDinf;
	unsigned int grr;
};

struct aluno *getAluno1();

// retorne NULL se o aluno2 não existir.
struct aluno *getAluno2();

void imprimirDadosAlunos();

// retorna NULL se não foi possível inserir.
struct nodo *inserir(struct nodo **raiz, int chave);

// retorna 1 caso removido, ou 0 caso não seja possível remover.
int excluir(struct nodo **raiz, int chave);

// retorna NULL se não existe.
struct nodo *buscar(struct nodo *nodo, int chave);

void imprimirEmOrdem(struct nodo *nodo);

void imprimirEmLargura(struct nodo *raiz);

void destroiArvore(struct nodo *nodo);

#endif // AVL_H_
