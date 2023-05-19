#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define BUFFER_SIZE 1024

void nomeArquivo(FILE *binario, char nome[]) {
	fgets(nome, BUFFER_SIZE, binario);
	char *newline = strchr(nome, '\n');
	*newline = '\0';
}

void extraiArquivos(FILE* binario) {
	unsigned int numRegistros, tamRegistro;
	char nome[BUFFER_SIZE];

	fread(&numRegistros, sizeof(unsigned int), 1, binario);

	for(unsigned int i = 0; i < numRegistros; i++) {
		fread(&tamRegistro, sizeof(unsigned int), 1, binario);
		nomeArquivo(binario, nome);

		char *conteudo = (char*) malloc(tamRegistro - strlen(nome) - 1);
		fread(conteudo, 1, tamRegistro - strlen(nome) - 1, binario);

		FILE *arquivo;

		arquivo = fopen(nome, "wb");
		if(!arquivo) {
			perror("Erro ao abrir arquivo");
			free(conteudo);
			exit(EXIT_FAILURE);
		}

		fwrite(conteudo, 1, tamRegistro - strlen(nome) - 1, arquivo);

		free(conteudo);
		fclose(arquivo);
	}
}

void imprimeArquivos(FILE *binario) {
	unsigned int numRegistros, tamRegistro;
	char nome[BUFFER_SIZE];

	fread(&numRegistros, sizeof(unsigned int), 1, binario);

	for(unsigned int i = 0; i < numRegistros; i++) {
		fread(&tamRegistro, sizeof(unsigned int), 1, binario);
		nomeArquivo(binario, nome);
		printf("%s, %u bytes\n", nome, (unsigned int)(tamRegistro - strlen(nome) - 1));
		fseek(binario, tamRegistro - strlen(nome) - 1, SEEK_CUR);
	}
}

int main(int argc, char **argv) {
	int opt, opt_t = 0;

	while((opt = getopt(argc, argv, "t")) != -1) {
		switch (opt) {
			case 't':
				opt_t = 1;
				break;

			default:
				fprintf(stderr, "Uso: ./backup [-t]\n");
				exit(EXIT_FAILURE);
		}
	}

	FILE *binario = fopen("bkp.bin", "rb");

	if(!binario) {
		perror("Erro ao abrir arquivo");
		exit(EXIT_FAILURE);
	}

	if(opt_t)
		imprimeArquivos(binario);
	else
		extraiArquivos(binario);

	fclose(binario);
	return 0;
}
