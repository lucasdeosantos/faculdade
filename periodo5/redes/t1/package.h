/*
========================================================================
Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
Date: 07/12/2024
========================================================================
*/

#ifndef PACKAGE_H
#define PACKAGE_H

#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include "socket.h"

// Definição de MACROS
#define CLIENT "cliente"
#define SERVER "servidor"
#define TIMEOUT 1 // Define o tempo de timeout em segundos
#define PACK_MAX 67
#define DATA_MAX 63
#define BEGIN_MK 0b01111110
#define MASK_01 0b11111100
#define MASK_02 0b00000011
#define MASK_03 0b11100000
#define MASK_04 0b00011111
#define GEN_CRC 0b11010101
#define POL_CRC 0b10000000

// Definição dos códigos de tipos
#define CODE_ACK 0b00000
#define CODE_NACK 0b00001
#define CODE_OK 0b00010
#define CODE_BKP 0b00100
#define CODE_CAT 0b00101
#define CODE_VER 0b00110
#define CODE_OK_CK 0b01101
#define CODE_OK_LEN 0b01110
#define CODE_LEN 0b01111
#define CODE_DATA 0b10000
#define CODE_DATA_END 0b10001
#define CODE_ERROR 0b11111

// Definição dos códigos de erro
#define ERROR_OPN 1
#define ERROR_MEM 2
#define ERROR_FND 3
#define ERROR_CKS 4
#define ERROR_SKT 5

// Alias para não serem utilizados tipos diretamente
typedef bool validate_t;
typedef char byte_t;

/*
 * Estrutura para representar o pacote de rede
 *      begin_mk: Marcador de início com 1 byte
 *      lgth_seq_tp: 2 bytes para representar Tamanho + Sequência + Tipo
 *      data: 63 bytes para dados (é variável o tamanho)
 *      crc: CRC de 1 byte
*/
struct package {
    byte_t len;
    byte_t seq;
    byte_t type;
    byte_t* data;
    byte_t crc;
};

// Funcão que valida a integridade do pacote
validate_t verifyPackage (struct package* pack);

// Função que calcula o checksum de um arquivo
size_t calculateCheckSum(FILE *file);

// Função que destrói o pacote
struct package* destroyPackage (struct package* pack);

//Função que realiza o recebimento de um pacote
struct package* receivePackage (struct socket_st* sock);

//Função que realiza o envio de um pacote de ACK
void sendACK (struct socket_st* sock);

//Função que realiza o envio de um pacote de NACK
void sendNACK (struct socket_st* sock);

// Função que realiza o envio de um pacote de OK
void sendOK (struct socket_st* sock);

// Função que realiza o envio de um pacote de BKP
void sendBackupRequest (struct socket_st* sock, byte_t* file_path);

// Função que realiza o envio de um pacote de CAT
void sendCateringRequest (struct socket_st* sock, byte_t *file_path); 

// Função que realiza o envio de um pacote de VER
void sendVerifyRequest (struct socket_st* sock, byte_t *file_path);

// Função que realiza o envio de um pacote de OK_CK
void sendOKandCK (struct socket_st* sock, size_t ck_sum);

// Função que realiza o envio de um pacote de OK_LEN
void sendOKandLEN (struct socket_st* sock, size_t length);

// Função que realiza o envio de um pacote de LEN
void sendFileLength (struct socket_st* sock, size_t length);

// Função que realiza o envio de um pacote de DATA
void sendData (struct socket_st* sock, byte_t len, byte_t seq, byte_t *data);

// Função que realiza o envio de um pacote de DATA_END
void sendDataEnd (struct socket_st* sock);

//Função que realiza o envio de um pacote de ERROR
void sendError (struct socket_st* sock, byte_t error_number);

// Função que calcula o timeout 
bool timeout (time_t begin);

// Função que imprime um código de erro
void printError(byte_t error, char* org);

// Função que verifica se existe memória disponível no disco
bool verifyMemoryAvailable(size_t length);

// Função que retorna o nome da primeira interface válida de rede
char* getInterfaceName();

#endif // PACKAGE_H
