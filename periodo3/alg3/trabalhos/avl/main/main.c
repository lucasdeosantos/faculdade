#include "avl.h"

int main()
{
	imprimirDadosAlunos();

	struct nodo *raiz = NULL;
	int chave;
	char c;

	while ((c = getchar())) {
		switch (c) {
		case 'i':
			if ((scanf("%d", &chave) != 1))
				break;
			if (!(inserir(&raiz, chave)))
				printf("Falha ao inserir %d.\n", chave);
			break;
		case 'r':
			if ((scanf("%d", &chave) != 1))
				break;
			if (!(excluir(&raiz, chave)))
				printf("Falha ao remover %d.\n", chave);
			break;
		case 'b':
			if ((scanf("%d", &chave) != 1))
				break;
			if (!(buscar(raiz, chave))) {
				printf("Não encontrado %d.\n", chave);
				break;
			}
			printf("Encontrado %d.\n", chave);
			break;
		case 'e':
			imprimirEmOrdem(raiz);
			break;
		case 'l':
			imprimirEmLargura(raiz);
			break;
		case 'f':
			destroiArvore(raiz);
			return 0;
		default:
			printf("Uso: i val, r val, b val, e, l, f.\n");
			break;
		}
		// limpa o buffer do teclado.
		while (getchar() != '\n');
	}
	return 0;
}
