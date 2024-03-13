#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "libpilha.h"

typedef double t_operador;

#define TAM_ENTRADA 256
#define TAM_PILHA 1024


/* Constantes com valores para identificar os operadores. O valor 
   antes do ponto flutuante difine a precedencia entre os operadores, 
   valores maiores tem maior precedencia. */
#define SOM 10.1
#define SUB 10.2
#define MUL 20.1
#define DIV 20.2
#define POW 30.1


/* Identificador de '(' para ser empilhado na pilha de operadores */
#define PAR 99.0


/* Converte os caracteres que representam os operadores na entrada
   para valores constantes que identificam os operadores. 
   Retorna 1 se o caracter c representa um operador valido e 0 caso
   contrario. */
int converte_operador(t_operador *op, char c) {
	if(c == '+')
		*op = SOM;
	else if(c == '-')
		*op = SUB;
	else if(c == '*')
		*op = MUL;
	else if(c == '/')
		*op = DIV;
	else if (c == '^')
		*op = POW;
	else
		return 0;
	return 1;
}


/* Retorna 1 se o operador op1 tem precedencia sobre o operador op2.
   Retorna 0 caso contrario. */
int precede(t_operador op1, t_operador op2) {
    if((op1 - op2) >= 1.0)
        return 1;
    return 0;
}


/* Desempilha os dois valores no topo da pilha de valores, aplica o
   operador sobre esses valores e empilha o resultado na pilha de 
   valores. */
int opera(t_operador op, t_pilha *valores) {
    double val_esq, val_dir;

    if(!desempilha(&val_dir, valores))
        return 0;
    if(!desempilha(&val_esq, valores))
        return 0;
    if(op == SOM)
        return empilha(val_esq + val_dir, valores);
    if(op == SUB)
        return empilha(val_esq - val_dir, valores);
    if(op == MUL)
        return empilha(val_esq * val_dir, valores);
    if(op == DIV && val_dir != 0.0)
        return empilha(val_esq / val_dir, valores);
    if(op == POW && val_dir)
		return empilha(pow(val_esq, val_dir), valores);
    return 0;
}


/* Imprime na saida de erro (stderr) a mensagem de erro e a linha e 
   a coluna da entrada onde o erro foi detectado e aborta o programa. */
void erro_aborta(char *msg, int col) {
	fprintf(stderr, "ERRO: %s (coluna %d)\n", msg, col);
	exit(1);
}

/* Imprime na saida de erro (stderr) a mensagem de erro e a linha e 
   a coluna da entrada onde o erro foi detectado. */
void erro_imprime(char *msg, int col) {
	fprintf(stderr, "ERRO: %s (coluna %d)\n", msg, col);
}


/* Le da entrada padrao, usando fgets, um vetor de caracteres ate o \n.
   Se o tamanho da entrada for maior que o vetor de leitura, aumenta
   o tamanho do vetor e continua a leitura ate encontrar o \n.
   Retorna um ponteiro para o vetor lido ou NULL caso ocorra algum erro
   ne leitura ou ne alocação de memoria. */
/*
char* le_entrada() {
    char *ent, *ret_realloc, *ret_fgets;
    int tam, pos;

    ent = NULL;
    tam = 0;
    pos = 0;
    
    do {
        tam += TAM_ENTRADA;
        ret_realloc = (char*) realloc(ent, sizeof(char) * tam);
        if(!ret_realloc) {
            free(ent);
            return NULL;
        }
        ent = ret_realloc;
        ent[tam - 1] = ' ';
        ret_fgets = fgets(ent + pos, tam - pos, stdin);
        pos = tam - 1;    
    } while(ret_fgets && ent[tam - 1] == '\0' && ent[tam - 2] != '\n');
    
    if(!ret_fgets && tam == TAM_ENTRADA) {
        free(ent);
        return NULL;
    }
    
    return ent;
}
*/


/* Le da entrada padrao, usando fgets, um vetor de caracteres ate o \n.
   Retorna um ponteiro para o vetor lido ou NULL caso ocorra algum erro
   ne leitura ou na alocação de memoria. Se o tamanho da entrada for 
   maior que o vetor de leitura, retorna NULL. */
char* le_entrada() {
    char *ent, *ret_fgets;
    int tam;
    
    tam = TAM_ENTRADA;
    ent = (char*) malloc(sizeof(char) * tam);
    if(!ent)
        return NULL;
    ent[tam - 1] = ' ';
    ret_fgets = fgets(ent, tam, stdin);
    if(!ret_fgets || (ent[tam - 1] == '\0' && ent[tam - 2] != '\n')) {
        free(ent);
        return NULL;
    }
    return ent;
}


int main() {
	t_pilha *pilha_valores, *pilha_operadores;
	t_operador operador, op_topo;
	double operando, resultado;
	char *entrada, *c, *prox;
	int erro, quit;


	/* Se ocorrer um erro de memoria na criacao das pilhas, aborta *
	 * o programa para evitar leak de memoria. */
	pilha_valores = cria_pilha(TAM_PILHA);
	if(!pilha_valores) 
		erro_aborta("erro ao criar pilha de valores", 0);

	pilha_operadores = cria_pilha(TAM_PILHA);
	if(!pilha_operadores) {
		if(pilha_valores)
			destroi_pilha(pilha_valores);
		erro_aborta("erro ao criar pilha de operadores", 0);
	}

	quit = 0;
	resultado = 0;
	/* Enquanto o usuario nao acionar a condicao de parada, o *
	 * programa continua rodando. */
	while(!quit) {

		/* Se ocorrer um erro de leitura da entrada, aborta o programa *
		 * para evitar leak de memoria */
		entrada = le_entrada();
		if(!entrada) 
			erro_aborta("erro de leitura", 0);
		
		erro = 0;
		c = entrada;
		/* Enquanto o usuario nao apertar ENTER, nao der erro e a condicao *
		 * de parada nao for acionada, o programa continua rodando. */
		while(*c != '\n' && !erro && !quit) {
			/* Percorre o ponteiro c pela entrada ate o final de linha. */

			/* Caso 1: separadores */
			if(*c == ' ' || *c == '\t')
				/* Se for sepador, passa para o proximo caracter. */
				c++;

			/* Caso 2: operando */
			else if(isdigit(*c)) {
				/* Se for [1..9] le um valor e empilha na pilha de valores. */
				operando = strtod(c, &prox);
				if(c == prox) {
					erro_imprime("operando incorreto", c - entrada + 1);
					erro = 1;
				}
				if(!empilha(operando, pilha_valores)) {
					erro_imprime("pilha de valores cheia", c - entrada + 1);
					erro = 1;
				}
				c = prox;
			}

			/* Caso 3: memória */
			else if(*c == 'm') {
				/* Empilha o valor da memoria na pilha de valores. */
				if(!empilha(resultado, pilha_valores)) {
					erro_imprime("pilha de valores cheia", c - entrada + 1);
					erro = 1;
				}
				c++;
			}

			/* Caso 4: abre parenteses */
			else if(*c == '(') {
				/* Se for abre parenteses, empilha PAR na pilha de operadores. */
				if(!empilha(PAR, pilha_operadores)) {
					erro_imprime("pilha de operadores cheia", c - entrada + 1);
					erro = 1;
				}
				c++;
			}

			/* Caso 5: fecha parenteses */
			else if(*c == ')') {
				/* Se for fecha parenteses, processa a pilha de operadores até 
				   encontar um PAR. */ 
				while(topo(&op_topo, pilha_operadores) && op_topo != PAR) {
					desempilha(&op_topo, pilha_operadores);
					if(!opera(op_topo, pilha_valores)) {
						erro_imprime("formato incorreto", c - entrada + 1);
						erro = 1;
					}
				}
				if(pilha_vazia(pilha_operadores) ||
						(desempilha(&op_topo, pilha_operadores) && op_topo != PAR)) {
					erro_imprime("formato incorreto", c - entrada + 1);
					erro = 1;
				}
				c++;
			}

			/* Caso 6: condicao de parada */
			/* Enquanto 'q' nao for digitado o programa nao para.*/
			else if(*c == 'q')
				quit = 1;

			/* Caso 7: operador */
			else if(converte_operador(&operador, *c)) {
				/* Se for um operador valido, verifica a precedencia em relacao
				   ao topo da pilha de operadores. */
				while(topo(&op_topo, pilha_operadores) &&
						op_topo != PAR &&
						!precede(operador, op_topo)) {
					/* Enquando o topo da pilha tiver precedencia, desempilha e
					   processa o operador do topo da pilha. */
					desempilha(&op_topo, pilha_operadores);
					if(!opera(op_topo, pilha_valores)) {
						erro_imprime("formato incorreto", c - entrada + 1);
						erro = 1;
					}
				}
				if(!empilha(operador, pilha_operadores)) {
					/* Empilha o novo operador na pilha de operadores. */
					erro_imprime("pilha de operadores cheia", c - entrada + 1);
					erro = 1;
				}
				c++;
			}

			/* Caso 8: caracter invalido na entrada */
			else {
				erro_imprime("caracter desconhecido", c - entrada + 1);
				erro = 1;
			}
		}

		/* Se a condicao de parada for acionada o programa nao imprime nada. */
		if(!quit) {
			/* Nesse ponto o processamento da entrada terminou e pode ser o caso da 
			   pilha de operadores nao estar vazia. */
			while(desempilha(&op_topo, pilha_operadores)) {
				/* Processa os operadores que restaram na pilha. */
				if(!opera(op_topo, pilha_valores) && !erro) {
					erro_imprime("formato incorreto", c - entrada + 1);
					erro  = 1;
				}
			}

			/* Se o topo da pilha de valores for maior do que 1 no final da execução, *
			 * houve algum erro, portanto, desempilha a pilha de valores. */
			if(pilha_valores->topo > 1) 
				while(desempilha(&op_topo, pilha_valores));

			/* após o processamento, o resultado final da expressao esta no topo da 
			   pilha de valores. */
			if((!desempilha(&resultado, pilha_valores) || !pilha_vazia(pilha_valores)) && !erro) {
				erro_imprime("formato incorreto", c - entrada + 1);
				erro = 1;
			}
			/* Se nao der nenhum erro nos processos anteriores imprime o resultado. */
			if(!erro)
				printf("%.16g\n", resultado);
		}

		free(entrada);
	}

	destroi_pilha(pilha_valores);
	destroi_pilha(pilha_operadores);

	return 0;
}
