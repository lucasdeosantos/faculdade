#include <stdio.h>
#include <stdlib.h>
#include "tabelaHash.h"

int main()
{
	imprimirDadosAlunos();

	size_t tamTabela;
	if (scanf("%zu ", &tamTabela) != 1) {
		fputs("Entrada invalida.\n", stderr);
		exit(1);
	}

	struct nodoHash **tabelaHash = gerarTabelaHash(tamTabela);
	if (!tabelaHash) {
		fputs("Falha ao alocar memoria.\n", stderr);
		exit(1);
	}

	int chave;
	char c;

	while ((c = getchar())) {
		switch (c) {
		case 'i':
			if (scanf("%d", &chave) != 1) {
				fputs("Entrada invalida.\n", stderr);
				break;
			}
			if (!(inserir(tabelaHash, tamTabela, chave)))
				printf("Falha ao inserir %d.\n", chave);
			break;
		case 'r':
			if (scanf("%d", &chave) != 1) {
				fputs("Entrada invalida.\n", stderr);
				break;
			}
			struct nodoHash *nodo = buscar(tabelaHash, tamTabela, chave);
			if (!nodo) {
				printf("Falha ao remover %d.\n", chave);
				break;
			}
			excluir(tabelaHash, tamTabela, nodo);
			break;
		case 'b':
			if (scanf("%d", &chave) != 1) {
				fputs("Entrada invalida.\n", stderr);
				break;
			}
			if (!(buscar(tabelaHash, tamTabela, chave))) {
				printf("Não encontrado %d.\n", chave);
				break;
			}
			printf("Encontrado %d.\n", chave);
			break;
		case 'l':
			imprimirTabelaHash(tabelaHash, tamTabela);
			break;
		case 'f':
			liberarTabelaHash(tabelaHash, tamTabela);
			return 0;
		}
		// limpa o buffer do teclado.
		while (getchar() != '\n');
	}
	return 1;
}
