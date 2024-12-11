/*
========================================================================
Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
Date: 22/11/2024
========================================================================
*/

#include "socket.h"

// Função que associa a interface física ao socket
itf_id_type interfaceConfig(const char* interface_name)
{
    itf_id_type itf_id = if_nametoindex(interface_name);

    return itf_id;
}


// Função que cria um socket
struct socket_st* createSocket (const char* interface_name)
{    
    struct socket_st* sck = malloc(sizeof(struct socket_st));

    sck->socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sck->socket == -1) {
        perror("Erro ao criar socket: Verifique se você é root!\n");
        exit(-1);
    }

    sck->itf_id = interfaceConfig(interface_name);

    struct sockaddr_ll address = {0};
    address.sll_family = AF_PACKET;
    address.sll_protocol = htons(ETH_P_ALL);
    address.sll_ifindex = sck->itf_id;
    sck->address = address;

    if (bind(sck->socket, (struct sockaddr*)&sck->address, sizeof(sck->address)) == -1) {
        fprintf(stderr, "Erro ao fazer bind no socket\n");
        exit(-1);
    }

    struct packet_mreq mr = {0};
    mr.mr_ifindex = sck->itf_id;
    mr.mr_type = PACKET_MR_PROMISC;
    if (setsockopt(sck->socket, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
        fprintf(stderr, "Erro ao fazer setsockopt: "
            "Verifique se a interface de rede foi especificada corretamente.\n");
        exit(-1);
    }

    return sck;
}
