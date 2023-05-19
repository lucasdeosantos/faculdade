#include "beale.h"

void imprimeErro(char nomePrograma[]){
	fprintf(stderr, "Uso: %s -e -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves\n", nomePrograma);
	fprintf(stderr, "Uso: %s -d -i MensagemCodificada -c ArquivoDeChaves -o MensagemDecodificada\n", nomePrograma);
	fprintf(stderr, "Uso: %s -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada\n", nomePrograma);
	return;
}

int main(int argc, char **argv) {
	int opt, opt_e = 0, opt_d = 0, opt_b = 0, opt_m = 0, opt_o = 0, opt_c = 0, opt_i = 0;
	char *argv_b, *argv_m, *argv_o, *argv_c, *argv_i; 
	lista_t **cifras;

	srand(time(NULL));

	if(argc < 8 || argc > 10) {
		imprimeErro(argv[0]);
		exit(EXIT_FAILURE);
	}

	while((opt = getopt(argc, argv, "edb:m:o:c:i:")) != -1) {
		switch(opt) {
			case 'e':
				opt_e = 1;
				break;

			case 'd':
				opt_d = 1;
				break;

			case 'b':
				opt_b = 1;
				argv_b = optarg;
				break;

			case 'm':
				opt_m = 1;
				argv_m = optarg;
				break;

			case 'o':
				opt_o = 1;
				argv_o = optarg;
				break;

			case 'c':
				opt_c = 1;
				argv_c = optarg;
				break;

			case 'i':
				opt_i = 1;
				argv_i = optarg;
				break;

			default:
				imprimeErro(argv[0]);
					exit(EXIT_FAILURE);
			}
	}

	cifras = criaVetorCifras();

	if(opt_e) {
		FILE *LivroCifra, *MensagemOriginal;

		LivroCifra = abreArquivo(argv_b, "r");
		MensagemOriginal = abreArquivo(argv_m, "r");

		if(opt_b && opt_m && opt_o) {
			geraCifras(LivroCifra, cifras);
			geraArquivoCodificado(MensagemOriginal, argv_o, cifras);
			if(opt_c)
				geraArquivoChaves(argv_c, cifras);
		}
		fechaArquivo(LivroCifra);
		fechaArquivo(MensagemOriginal);
	}
	else if(opt_d) {
		FILE *MensagemCodificada;

		MensagemCodificada = abreArquivo(argv_i, "r");

		if(opt_i && opt_c && opt_o) {
			FILE *ArquivoDeChaves;

			ArquivoDeChaves = abreArquivo(argv_c, "r");
		
			geraCifrasArquivo(ArquivoDeChaves, cifras);
			geraArquivoDecodificado(MensagemCodificada, argv_o, cifras);

			fechaArquivo(ArquivoDeChaves);
		}
		if(opt_i && opt_b && opt_o) {
			FILE *LivroCifra;

			LivroCifra = abreArquivo(argv_b, "r");

			geraCifras(LivroCifra, cifras);
			geraArquivoDecodificado(MensagemCodificada, argv_o, cifras);

			fechaArquivo(LivroCifra);
		}
		fechaArquivo(MensagemCodificada);
	}
	else {
		imprimeErro(argv[0]);
		exit(EXIT_FAILURE);
	}

	destroiVetorCifras(cifras);

	return 0;
}
