#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#include <stddef.h>

struct nodoHash{
	int chave;
	struct nodoHash *anterior;
	struct nodoHash *proximo;
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

// aloca a tabela hash na memória.
struct nodoHash **gerarTabelaHash(size_t tamTabela);

// insere a chave na tabela hash, duplicatas não são permitidas.
// retorna o nodo inserido, ou NULL em caso de problemas (ex: caso a chave seja uma duplicata).
struct nodoHash *inserir(struct nodoHash *tabelaHash[], size_t tamTabela, int chave);

// deleta a tabela hash.
void liberarTabelaHash(struct nodoHash *tabelaHash[], size_t tamTabela);

// imprime na tela a tabela completa.
void imprimirTabelaHash(struct nodoHash *tabelaHash[], size_t tamTabela);

// busca o nodo com a chave, ou retorna NULL caso tal nodo não exista.
struct nodoHash *buscar(struct nodoHash *tabelaHash[], size_t tamTabela, int chave);

// exclui o nodo apontado.
void excluir(struct nodoHash *tabelaHash[], size_t tamTabela, struct nodoHash *nodo);

#endif // TABELA_HASH_H
