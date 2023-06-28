#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum { NOP, ENCODE, DECODE } Modo_t;

#define BUFFER_SIZE 10

// PROTÓTIPOS

// Função que comprime 'fIn', gravando o resultado da compressão em 'fOut'
void Encode (char *input, char *output);

// Função que descomprime 'fIn', gravando o resultado da descompressão em 'fOut'
void Decode (char *input, char *output);

// Função  ser  usada  no  programa para  chamar  Encode()  ou  Decode()
void rle (void (func) (char*, char*), char *input, char *output);


// PROGRAMA PRINCIPAL
int main(int argc, char *argv[])
{
	int opt;
	char *input = NULL;
	char *output = NULL;
	Modo_t modo = NOP;

	while ((opt = getopt(argc, argv, "dei:o:")) != -1) {
		switch (opt) {
			case 'i':
				input = strdup(optarg);
				break;
			case 'o':
				output = strdup(optarg);
				break;
			case 'e':
				modo |= ENCODE;
				break;
			case 'd':
				modo |= DECODE;
				break;
			default:
				fprintf(stderr, "Forma de uso: ./rle [-e | -d] -i <arq_in> -o <arq_out>\n");
				exit(1);
		}		
	}

	// Erro se:
	//  * não forneceu os dois arquivos
	//  * não forneceu uma das opções '-e' ou '-d',
	//  * forneceu as opções '-e' e '-d' na mesma linha de comando
	if ( ! input || ! output || modo == NOP || modo & (modo-1) ) {
		fprintf(stderr, "Forma de uso: ./rle [-e | -d] -i <arq_in> -o <arq_out>\n");
		exit(2);
	}

	if (modo == ENCODE)
		rle(Encode, input, output);
	else
		rle(Decode, input, output);

	return 0;
}


/* DEFINA ABAIXO O CÓDIGO DAS FUNÇÕES Encode(), Decode() e rle()  */

// Função ser usada no programa principal para chamar Encode() ou Decode()
void rle (void (func) (char*, char*), char *input, char *output)
{
	return func(input, output);
}



// Função que comprime 'fIn', gravando o resultado da compressão em 'fOut'
void Encode (char *input, char *output)
{
	FILE *fIn = fopen(input, "rb");
	if (!fIn) {
		perror("Erro ao abrir arquivo");
		exit(1);
	}

	FILE *fOut = fopen(output, "wb");
	if (!fOut) {
		perror("Erro ao abrir arquivo");
		exit(1);
	}

	char c;
	char pos;
	char count = 1;

	fread(&c, sizeof(char), 1, fIn);

	while((fread(&pos, sizeof(char), 1, fIn)) != EOF) {
		if (c == pos) {
			count++;
		}
		else {
			fwrite(&c, sizeof(char), 1, fOut);
			fwrite(&count, sizeof(char), 1, fOut);
			c = pos;
			count = 1;
		}
	}

	fclose(fIn);
	fclose(fOut);
}



// Função que descomprime 'fIn', gravando o resultado da descompressão em 'fOut'
void Decode (char *input, char *output)
{
	FILE *fIn = fopen(input, "rb");
	if (!fIn) {
		perror("Erro ao abrir arquivo");
		exit(1);
	}

	FILE *fOut = fopen(output, "wb");
	if (!fOut) {
		perror("Erro ao abrir arquivo");
		exit(1);
	}

	char c;
	char count;

	while((fread(&c, sizeof(char), 1, fIn)) != EOF && (fread(&count, sizeof(char), 1, fIn)) != EOF) {
		for(short i = 0; i < (short)count; i++)
			fwrite(&c, sizeof(char), 1, fOut);
	}

	fclose(fIn);
	fclose(fOut);
}
