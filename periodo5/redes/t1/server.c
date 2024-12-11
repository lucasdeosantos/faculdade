/*
========================================================================
Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
Date: 09/12/2024
========================================================================
*/

#include <sys/stat.h>
#include "package.h"

// Função que responde a requisição do backup
void backupFileAnswer (struct socket_st* sock, byte_t* file_path)
{
    struct package* rcv_pack;
    FILE* file;
    size_t len_file, div, mod;
    time_t begin;
    byte_t prev_seq;

    file = fopen(file_path, "w");
    if (file == NULL) {
        printError(ERROR_OPN, SERVER);
        while (1) {
            begin = time(NULL);
            sendError(sock, ERROR_OPN);
            while (!timeout(begin)) {
                rcv_pack = receivePackage(sock);
                if (rcv_pack != NULL) {
                    if (verifyPackage(rcv_pack))
                        break;
                    else
                        sendNACK(sock);
                }
            }
            if ((rcv_pack != NULL && rcv_pack->type == CODE_ACK) || timeout(begin)) break;
        }
        if (rcv_pack) destroyPackage(rcv_pack);
        return;
    }

    // Envia CODE_OK e recebe CODE_LEN
    while (1) {
        begin = time(NULL);
        sendOK(sock);
        while (!timeout(begin)) {
            rcv_pack = receivePackage(sock);
            if (rcv_pack != NULL) {
                if (verifyPackage(rcv_pack))
                    break;
                else
                    sendNACK(sock);
            }
        }
        if (rcv_pack != NULL && rcv_pack->type == CODE_LEN) break;
    }

    memcpy(&len_file, rcv_pack->data, sizeof(size_t));
    destroyPackage(rcv_pack);

    // Verifica se existe memória disponível
    if (!verifyMemoryAvailable(len_file)) {
        printError(ERROR_MEM, SERVER);
        while (1) {
            begin = time(NULL);
            sendError(sock, ERROR_MEM);
            while (!timeout(begin)) {
                rcv_pack = receivePackage(sock);
                if (rcv_pack != NULL) {
                    if (verifyPackage(rcv_pack))
                        break;
                    else
                        sendNACK(sock);
                }
            }
            if ((rcv_pack != NULL && rcv_pack->type == CODE_ACK) || timeout(begin)) break;
        }
        if (rcv_pack) destroyPackage(rcv_pack);
        return;
    }

    // Envia CODE_OK e recebe CODE_DATA
    while (1) {
        begin = time(NULL);
        sendOK(sock);
        while (!timeout(begin)) {
            rcv_pack = receivePackage(sock);
            if (rcv_pack != NULL) {
                if (verifyPackage(rcv_pack))
                    break;
                else
                    sendNACK(sock);
            }
        }
        if (rcv_pack != NULL && rcv_pack->type == CODE_DATA) break;
    }

    div = len_file / DATA_MAX;
    mod = len_file % DATA_MAX;

    prev_seq = rcv_pack->seq;
    // Recebe o arquivo
    if (div > 0) {
        for (int i = 0; i < div; ++i){
            fwrite(rcv_pack->data, sizeof(byte_t), DATA_MAX, file);
            destroyPackage(rcv_pack);

            while (1) {
                begin = time(NULL);
                sendACK(sock);
                while (!timeout(begin)) {
                    rcv_pack = receivePackage(sock);
                    if (rcv_pack != NULL) {
                        if (verifyPackage(rcv_pack) && rcv_pack->type == CODE_DATA && rcv_pack->seq != prev_seq)
                            break;
                        else
                            sendNACK(sock);
                    }
                }
                if (rcv_pack != NULL && (rcv_pack->type == CODE_DATA || rcv_pack->type == CODE_DATA_END)) break;
            }
            prev_seq = rcv_pack->seq;
        }
    }

    if (mod > 0) {
        fwrite(rcv_pack->data, sizeof(byte_t), mod, file);
        destroyPackage(rcv_pack);
    }

    sendACK(sock);
    fclose(file);
    return;
}


// Função que responde a requisição de restauração
void cateringFileAnswer (struct socket_st* sock, byte_t* file_path)
{
    struct package* rcv_pack;
    struct stat info_file;
    FILE* file;
    size_t div, mod;
    byte_t* buffer;
    time_t begin;

    file = fopen(file_path, "r");
    if (file == NULL) {
        printError(ERROR_OPN, SERVER);
        while (1) {
            begin = time(NULL);
            sendError(sock, ERROR_OPN);
            while (!timeout(begin)) {
                rcv_pack = receivePackage(sock);
                if (rcv_pack != NULL) {
                    if (verifyPackage(rcv_pack))
                        break;
                    else
                        sendNACK(sock);
                }
            }
            if ((rcv_pack != NULL && rcv_pack->type == CODE_ACK) || timeout(begin)) break;
        }
        if (rcv_pack) destroyPackage(rcv_pack);
        return;
    }

    // Envia CODE_OK_LEN e recebe CODE_OK
    stat(file_path, &info_file);
    while (1) {
        begin = time(NULL);
        sendOKandLEN(sock, info_file.st_size);
        while (!timeout(begin)) {
            rcv_pack = receivePackage(sock);
            if (rcv_pack != NULL) {
                if (verifyPackage(rcv_pack))
                    break;
                else
                    sendNACK(sock);
            }
        }
        if (rcv_pack != NULL && (rcv_pack->type == CODE_OK || rcv_pack->type == CODE_ERROR)) break;
    }
    if (rcv_pack->type == CODE_ERROR) {
        printError(rcv_pack->data[0], CLIENT);
        destroyPackage(rcv_pack);
        sendACK(sock);
        return;
    }
    destroyPackage(rcv_pack);

    div = info_file.st_size / DATA_MAX;
    mod = info_file.st_size % DATA_MAX;

    // Envia o arquivo
    if (div > 0){
        for (int i = 0; i < div; ++i){
            buffer = malloc(DATA_MAX * sizeof(byte_t));
            fread(buffer, sizeof(byte_t), DATA_MAX, file);

            while (1) {
                begin = time(NULL);
                sendData(sock, DATA_MAX, (byte_t)(i % 32), buffer);
                while (!timeout(begin)) {
                    rcv_pack = receivePackage(sock);
                    if (rcv_pack != NULL) {
                        if (verifyPackage(rcv_pack))
                            break;
                        else
                            sendNACK(sock);
                    }
                }
                if (rcv_pack != NULL && rcv_pack->type == CODE_ACK) break;
            }
            destroyPackage(rcv_pack);
            free(buffer);
        }
    }

    if (mod > 0){
        buffer = malloc(mod * sizeof(byte_t));
        fread(buffer, sizeof(byte_t), mod, file);

        while (1) {
            begin = time(NULL);
            sendData(sock, mod, (byte_t)(div % 32), buffer);
            while (!timeout(begin)) {
                rcv_pack = receivePackage(sock);
                if (rcv_pack != NULL) {
                    if (verifyPackage(rcv_pack))
                        break;
                    else
                        sendNACK(sock);
                }
            }
            if (rcv_pack != NULL && rcv_pack->type == CODE_ACK) break;
        }
        destroyPackage(rcv_pack);
        free(buffer);
    }

    // Envia CODE_DATA_END e recebe CODE_ACK
    while (1) {
        begin = time(NULL);
        sendDataEnd(sock);
        while (!timeout(begin)) {
            rcv_pack = receivePackage(sock);
            if (rcv_pack != NULL) {
                if (verifyPackage(rcv_pack))
                    break;
                else
                    sendNACK(sock);
            }
        }
        if ((rcv_pack != NULL && rcv_pack->type == CODE_ACK) || timeout(begin)) break;
    }

    fclose(file);
    return;
}


// Função que responde a requisição de comparação do arquivo local com o arquivo do servidor
void verifyFileAnswer (struct socket_st* sock, byte_t* file_path)
{
    struct package* rcv_pack;
    FILE* file;
    time_t begin;

    file = fopen(file_path, "r");
    if (file == NULL) {
        printError(ERROR_FND, SERVER);
        while (1) {
            begin = time(NULL);
            sendError(sock, ERROR_FND);
            while (!timeout(begin)) {
                rcv_pack = receivePackage(sock);
                if (rcv_pack != NULL) {
                    if (verifyPackage(rcv_pack))
                        break;
                    else
                        sendNACK(sock);
                }
            }
            if ((rcv_pack != NULL && rcv_pack->type == CODE_ACK) || timeout(begin)) break;
        }
        if (rcv_pack) destroyPackage(rcv_pack);
        return;
    }

    size_t checksum = calculateCheckSum(file);
    if (checksum == -1) printError(ERROR_CKS, SERVER);
    // Envia CODE_OK_CK e recebe CODE_ACK
    while (1) {
        begin = time(NULL);
        if (checksum == -1) sendError(sock, ERROR_CKS);
        else sendOKandCK(sock, checksum);
        while (!timeout(begin)) {
            rcv_pack = receivePackage(sock);
            if (rcv_pack != NULL) {
                if (verifyPackage(rcv_pack))
                    break;
                else
                    sendNACK(sock);
            }
        }
        if ((rcv_pack != NULL && rcv_pack->type == CODE_ACK) || timeout(begin)) break;
    }
    if (rcv_pack) destroyPackage(rcv_pack);
    return;
}


int main() {
    const char* interface_name = getInterfaceName();
    struct socket_st* sock = createSocket(interface_name);
    struct package* rcv_pack;
    byte_t file_path[DATA_MAX];
    time_t begin;

    if (sock->socket == -1) {
        printError(ERROR_SKT, "não foi possível criar o socket");
        return 0;
    }

    while (1) {
        rcv_pack = receivePackage(sock);
        if (rcv_pack != NULL) {
            if (verifyPackage(rcv_pack) == false) {
                while (1) {
                    begin = time(NULL);
                    sendNACK(sock);
                    while (!timeout(begin)) {
                        rcv_pack = receivePackage(sock);
                        if (rcv_pack != NULL) break;
                    }
                    if (rcv_pack != NULL) break;
                }
            }
            if (rcv_pack->type == CODE_BKP || rcv_pack->type == CODE_CAT || rcv_pack->type == CODE_VER){
                memcpy(file_path, rcv_pack->data, rcv_pack->len);
                if (rcv_pack->type == CODE_BKP) {
                    backupFileAnswer(sock, file_path);
                }
                else if (rcv_pack->type == CODE_CAT) {
                    cateringFileAnswer(sock, file_path);
                }
                else if (rcv_pack->type == CODE_VER) {
                    verifyFileAnswer(sock, file_path);
                }
            }
        }
    }
    return 0;
}
