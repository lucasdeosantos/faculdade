#include <getopt.h>
#include "vinapp.h"
#include "diretorio.h"

int main (int argc, char **argv)
{
	char *target, *arquivo;
	int opt = getopt(argc, argv, "iarxcm:h");
	switch (opt) {
	case 'i':
	case 'a':
	case 'r':
	case 'x':
	case 'c':
		if (argc < 3) {
			fprintf(stderr, "Erro: numero de parametros invalido.\n");
			return 1;
		}
		arquivo = argv[2];
		break;
	case 'm':
		if (argc != 5) {
			fprintf(stderr, "Erro: numero de parametros invalido.\n");
			return 1;
		}
		arquivo = argv[3];
		target = optarg;
		break;
	case 'h':
		imprimirMensagemAjuda();
		return 0;
	default:
		fprintf(stderr, "Erro: opcao invalida.\n");
		return 1;
	}
	
	FILE *vpp = fopen(arquivo, "r+");
	if (!vpp) {
		vpp = fopen(arquivo, "w+");
		if (!vpp) {
			fprintf(stderr, "Erro ao abrir arquivo.\n");
			return 1;
		}
	}

	struct diretorio *dir = lerDiretorio(vpp);
	if (!dir) {
		fprintf(stderr, "Erro na leitura do diretorio.\n");
		fclose(vpp);
		return 1;
	}

	if (opt == 'i' || opt == 'a' || opt == 'r' || opt == 'm') {
		if (opt == 'i') {
			for (int i = 3; i < argc; i++)
				inserir(inserirSempre, vpp, dir, argv[i]);
		}
		else if (opt == 'a') {
			for (int i = 3; i < argc; i++)
				inserir(inserirMaisRecente, vpp, dir, argv[i]);
		}
		else if (opt == 'r') {
			for (int i = 3; i < argc; i++)
				remover(vpp, dir, argv[i]);
		}
		else { // opt == 'm'
			mover(vpp, dir, target, argv[4]);
		}
		escreverDiretorio(vpp, dir);
	}
	else { // opt == 'x' || opt == 'c'
		if (opt == 'x') {
			if (argc > 3) {
				for (int i = 3; i < argc; i++)
					extrair(vpp, dir, argv[i]);
			}
			else {
				for (struct nodoArquivo *nodo = dir->ini; nodo; nodo = nodo->prox)
					extrair(vpp, dir, nodo->arq->nome);
			}
		}
		else { // opt == 'c'
			imprimirDiretorio(dir);
		}
	}

	liberarDiretorio(dir);
	fclose(vpp);
	return 0;
}
