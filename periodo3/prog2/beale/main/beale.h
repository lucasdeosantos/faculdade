#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>
#include <getopt.h>

#define MAX_SIZE_ARRAY 256
#define MAX_SIZE_WORD 200

#define SPACE 32
#define LINEFEED 10
#define HASHTAG 35
#define DOISPONTOS 58

struct nodo {
    long chave;
    struct nodo *prox;
};
typedef struct nodo nodo_t;

struct lista {
    nodo_t *ini;
    unsigned int tamanho;
};
typedef struct lista lista_t;

/* Aloca memoria para uma lista, retorna NULL em caso de erro na 
 * alocação de memoria. */
lista_t *criaLista ();

/* Libera a memoria alocada para uma lista. */
lista_t *destroiLista (lista_t *l);

/* Insere uma chave em uma lista, retorna 1 em caso de sucesso, caso contrario 
 * retorna 0. */
int insereLista (lista_t *l, long chave);

/* Escreve o conteudo de uma lista em um arquivo enviado como parametro. */
void escreveListaArquivo(FILE *arquivo, lista_t *l);

/* Abre e retorna um arquivo. Se ocorrer um erro, escreve na 
 * saída padrão de erro e termina a execução do programa. */
FILE *abreArquivo(char nome[], char modo[]);

/* Fecha um arquivo enviado como parametro. */
int fechaArquivo (FILE *arquivo);

/* Aloca memoria para um vetor de lista_t de 256 posições, retorna 
 * NULL em caso de erro na alocação. */
lista_t **criaVetorCifras();

/* Libera a memoria alocada para o vetor de lista_t. */
lista_t **destroiVetorCifras(lista_t **cifras);

/* Insere chaves no vetor de listas, onde cada indice eh o seu 
 * respectivo caracter na tabela ASCII. Se existir uma lista 
 * para aquele caracter, insere na lista, senão, cria uma lista 
 * e insere a chave. retorna 1 caso a operação for bem sucedida, 
 * caso contrario retorna 0. */
int insereVetorCifras(lista_t **cifras, int i, long chave);

/* Gera o vetor de cifras a partir de um livro cifra enviado como 
 * parametro. */
void geraCifras(FILE *LivroCifra, lista_t **cifras);

/* Gera o vetor de cifras a partir de um arquivo de chaves enviado como 
 * parametro. */
void geraCifrasArquivo(FILE *ArquivoDeChaves, lista_t **cifras);

/* Abre um arquivo com o nome enviado como parametro e escreve o vetor de chaves 
 * no mesmo, fecha o arquivo no fim da execução da função. */
void geraArquivoChaves(char nome[], lista_t **cifras);

/* Abre um arquivo com o nome enviado como parametro e escreve a mensagem codificada
 * no mesmo, fecha o arquivo no fim da execução da função. */
void geraArquivoCodificado(FILE *MensagemOriginal, char nome[], lista_t **cifras);

/* Abre um arquivo com o nome enviado como parametro e escreve a mensagem decodificada 
 * no mesmo, fecha o arquivo no fim da execução da função. */
void geraArquivoDecodificado(FILE *MensagemCodificada, char nome[], lista_t **cifras);
