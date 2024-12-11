/*
========================================================================
Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
Date: 22/11/2024
========================================================================
*/

#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

// Alias para não serem utilizados tipos diretamente
typedef int socket_type;
typedef int itf_id_type;

/*
 * Estrutura para representar o soquete
 *      socket: inteiro que representa o soquete
 *      itf_id: inteiro que representa o id do soquete
 *      address: estrutura que armazena o endereço da interface de rede
*/
struct socket_st {
    socket_type socket;
    itf_id_type itf_id;
    struct sockaddr_ll address;
};

// Função que cria um socket
struct socket_st* createSocket (const char* interface_name);

#endif // SOCKET_H
