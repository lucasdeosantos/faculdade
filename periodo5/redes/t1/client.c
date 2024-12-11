/*
========================================================================
Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
Date: 09/12/2024
========================================================================
*/

#include <sys/stat.h>
#include <ctype.h>
#include "package.h"

// Função que realiza a requisição de backup
void backupFileRequest (struct socket_st* sock, char* file_path)
{
    struct package* rcv_pack;
    struct stat info_file;
    FILE* file;
    size_t div, mod;
    byte_t* buffer;
    time_t begin;

    file = fopen(file_path, "r");
    if (file == NULL) {
        printError(ERROR_OPN, CLIENT);
        return;
    }

    // Envia CODE_BKP e recebe CODE_OK
    while (1) {
        begin = time(NULL);
        sendBackupRequest(sock, file_path);
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
        printError(rcv_pack->data[0], SERVER);
        destroyPackage(rcv_pack);
        sendACK(sock);
        return;
    }
    destroyPackage(rcv_pack);

    // Envia CODE_LEN e recebe CODE_OK
    stat(file_path, &info_file);
    while (1) {
        begin = time(NULL); 
        sendFileLength(sock, info_file.st_size);
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
        printError(rcv_pack->data[0], SERVER);
        destroyPackage(rcv_pack);
        sendACK(sock);
        return;
    }
    destroyPackage(rcv_pack);

    div = info_file.st_size / DATA_MAX;
    mod = info_file.st_size % DATA_MAX;

    // Envia o arquivo
    if(div > 0){
        for(int i = 0; i < div; ++i){
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

// Função que realiza a requisição de restauração
void cateringFileRequest (struct socket_st* sock, char* file_path)
{
    struct package* rcv_pack;
    FILE* file;
    size_t len_file, div, mod;
    time_t begin;
    byte_t prev_seq;

    // Envia CODE_CAT e recebe CODE_OK_LEN
    while (1) {
        begin = time(NULL);
        sendCateringRequest(sock, file_path);
        while (!timeout(begin)) {
            rcv_pack = receivePackage(sock);
            if (rcv_pack != NULL) {
                if (verifyPackage(rcv_pack))
                    break;
                else
                    sendNACK(sock);
            }
        }
        if (rcv_pack != NULL && (rcv_pack->type == CODE_OK_LEN || rcv_pack->type == CODE_ERROR)) break;
    }
    if (rcv_pack->type == CODE_ERROR) {
        printError(rcv_pack->data[0], SERVER);
        destroyPackage(rcv_pack);
        sendACK(sock);
        return;
    }

    memcpy(&len_file, rcv_pack->data, sizeof(size_t));
    destroyPackage(rcv_pack);

    file = fopen(file_path, "w");
    if (file == NULL) {
        printError(ERROR_OPN, CLIENT);
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

    // Verifica se existe memória disponível
    if (!verifyMemoryAvailable(len_file)) {
        printError(ERROR_MEM, CLIENT);
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

    if (mod > 0){
        fwrite(rcv_pack->data, sizeof(byte_t), mod, file);
        destroyPackage(rcv_pack);
    }

    sendACK(sock);
    fclose(file);
    return;
}

// Função que realiza a requisição de comparação do arquivo local com o arquivo do servidor
void verifyFileRequest (struct socket_st* sock, char* file_path)
{
    struct package* rcv_pack;
    FILE* file;
    time_t begin;
    size_t rcv_checksum;

    file = fopen(file_path, "r");
    if (file == NULL) {
        printError(ERROR_OPN, CLIENT);
        return;
    }

    // Envia CODE_VER e recebe CODE_OK_CK
    while (1) {
        begin = time(NULL);
        sendVerifyRequest(sock, file_path);
        while (!timeout(begin)) {
            rcv_pack = receivePackage(sock);
            if (rcv_pack != NULL) {
                if (verifyPackage(rcv_pack)) 
                    break;
                else
                    sendNACK(sock);
            }
        }
        if (rcv_pack != NULL && (rcv_pack->type == CODE_OK_CK || rcv_pack->type == CODE_ERROR)) break;
    }
    if (rcv_pack->type == CODE_ERROR) {
        printError(rcv_pack->data[0], SERVER);
        destroyPackage(rcv_pack);
        sendACK(sock);
        return;
    }

    memcpy(&rcv_checksum, rcv_pack->data, sizeof(size_t));
    size_t checksum = calculateCheckSum(file);
    if (checksum == -1)
        printError(ERROR_CKS, CLIENT);
    else if (checksum == rcv_checksum)
        printf("OK\n");
    else
        printf("NOT OK\n");

    destroyPackage(rcv_pack);
    sendACK(sock);
    return;
}

int main() {
    const char* interface_name = getInterfaceName();
    struct socket_st* sock = createSocket(interface_name);
    char* file_path;
    char buffer[DATA_MAX];
    char request;

    if (sock->socket == -1) {
        printError(ERROR_SKT, "não foi possível criar o socket");
        return 0;
    }
    
    while(1) {
        printf("======================== Menu ========================\n");
        printf("b <nome_arquivo>: Solicita o backup de um aquivo\n");
        printf("c <nome_arquivo>: Solicita a restauração de um arquivo\n");
        printf("v <nome_arquivo>: Solicita a verificação de um arquivo\n");
        printf("q: sair do programa\n");
        printf("Escolha uma das opções acima: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro ao ler a entrada. Tente novamente.\n");
            continue;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        request = buffer[0];

        file_path = strchr(buffer, ' ');
        if (file_path != NULL) {
            file_path++;
            while (isspace((unsigned char)*file_path)) file_path++;
        }

        switch (request) {
            case 'b':
                backupFileRequest(sock, file_path);
                break;
            case 'c':
                cateringFileRequest(sock, file_path);
                break;
            case 'v':
                verifyFileRequest(sock, file_path);
                break;
            case 'q':
                close(sock->socket);
                return 0;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }
    return 0;
}
