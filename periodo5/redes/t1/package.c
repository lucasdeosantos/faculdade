/*
========================================================================
Authored by: Lucas Emanuel de Oliveira Santos & Thiago Fabricio de Mello
Contact: lucasde@ufpr.br & thiago.mello@ufpr.br
Date: 07/12/2024
========================================================================
*/

#include "package.h"

// Função que converte o pacote na sequência de bits correspondente 
byte_t* convertPack2BitSeq (struct package* pack)
{
    byte_t* pack_bit_seq;
    byte_t buffer[4 + pack->len];

    buffer[0] = BEGIN_MK;
    buffer[1] = ((pack->len << 2) & MASK_01) | ((pack->seq >> 3) & MASK_02);
    buffer[2] = ((pack->seq << 5) & MASK_03) | (pack->type & MASK_04);

    for (int i = 3; i < pack->len+3; i++)
        buffer[i] = pack->data[i-3];
    
    buffer[pack->len+3] = pack->crc;

    pack_bit_seq = buffer;

    return pack_bit_seq;
}


// Função que calcula o CRC
byte_t calculateCRC (struct package* pack)
{
    byte_t crc = 0;
    byte_t generator = GEN_CRC;

    byte_t* pack_bit_seq = convertPack2BitSeq(pack);

    for (int j = 1; j < pack->len+3; j++) {
        crc ^= pack_bit_seq[j];

        for (int i = 0; i < 8; i++) {
            if ((crc & POL_CRC) != 0)
                crc = (byte_t)((crc << 1 ^ generator));
            else
                crc <<= 1;
        }
    }

    return crc;
}


// Função que constrói um pacote a partir dos dados lidos do arquivo
struct package* buildPackage (byte_t len, byte_t seq, byte_t type, byte_t* data)
{
    struct package* pack = malloc(sizeof(struct package));

    pack->len = len;
    pack->seq = seq;
    pack->type = type;
    pack->data = malloc(sizeof(byte_t) * len);

    memset(pack->data, 0, len);
    memcpy(pack->data, data, len);

    pack->crc = calculateCRC(pack);

    return pack;
}


// Função que destrói o pacote
struct package* destroyPackage (struct package* pack)
{
    free(pack->data);
    free(pack);

    return NULL;
}


// Funcão que valida a integridade do pacote
validate_t verifyPackage (struct package* pack)
{
    byte_t crc_calc = calculateCRC(pack);

    if(crc_calc == pack->crc)
        return true;
    else
        return false;
}


// Função que calcula o checksum de um arquivo
size_t calculateCheckSum(FILE *file)
{
    size_t checksum = 0;
    int byte;

    while((byte = fgetc(file)) != EOF) checksum += (unsigned char)byte;

    if (ferror(file)) return -1;

    return checksum;
}


// Função que realiza o envio de um pacote
void sendPackage (struct socket_st* sock, struct package* pack)
{
    byte_t buffer[PACK_MAX];
    byte_t* pck_bit_seq = convertPack2BitSeq(pack);

    memset(buffer, 0, PACK_MAX);
    memcpy(buffer, pck_bit_seq, PACK_MAX);

    ssize_t send_bytes = sendto(sock->socket, buffer, PACK_MAX, 0, (struct sockaddr*)&sock->address, sizeof(sock->address));
    if (send_bytes < 0) {
        close(sock->socket);
        exit(1);
    }
}


// Função que realiza o recebimento de um pacote
struct package* receivePackage (struct socket_st* sock)
{
    socklen_t sockadd_len = sizeof(sock->address);
    byte_t buffer[PACK_MAX], len, seq, type, *data;

    ssize_t recv_bytes = recvfrom(sock->socket, buffer, PACK_MAX, 0, (struct sockaddr*)&sock->address, &sockadd_len);

    int i = 0;
    while(buffer[i] != BEGIN_MK && i < PACK_MAX)
        i++;

    if(recv_bytes != -1 && i != PACK_MAX - 1) {
        if (recv_bytes < 0 || i == PACK_MAX)
            return NULL;

        len = (buffer[i+1] & MASK_01) >> 2;
        seq = (buffer[i+1] & MASK_02) << 3 | (buffer[i+2] & MASK_03) >> 5;
        type = buffer[i+2] & MASK_04;    
        byte_t data_aux[len];

        memset(data_aux, 0, len);

        for (int j = i+3; j < i+len+3; j++)
            data_aux[j-i-3] = buffer[j];
        
        data = data_aux;

        return buildPackage(len, seq, type, data);
    }
    return NULL;
}


// Função que realiza o envio de um pacote de ACK
void sendACK (struct socket_st* sock)
{
    byte_t data[1] = {0};
    struct package* ack_pack = buildPackage(1, 0, CODE_ACK, data);
    sendPackage(sock, ack_pack);
    destroyPackage(ack_pack);
}


// Função que realiza o envio de um pacote de NACK
void sendNACK (struct socket_st* sock)
{
    byte_t data[1] = {0};
    struct package* nack_pack = buildPackage(1, 0, CODE_NACK, data);
    sendPackage(sock, nack_pack);
    destroyPackage(nack_pack);
}


// Função que realiza o envio de um pacote de OK
void sendOK (struct socket_st* sock)
{
    byte_t data[1] = {0};
    struct package* ok_pack = buildPackage(1, 0, CODE_OK, data);
    sendPackage(sock, ok_pack);
    destroyPackage(ok_pack);
}


// Função que realiza o envio de um pacote de BCK
void sendBackupRequest (struct socket_st* sock, byte_t* file_path)
{
    byte_t data[strlen(file_path) + 1];
    memcpy(data, file_path, strlen(file_path) + 1);
    struct package* bck_pack = buildPackage(strlen(file_path) + 1, 0, CODE_BKP, data);
    sendPackage(sock, bck_pack);
    destroyPackage(bck_pack);
}


// Função que realiza o envio de um pacote de CAT
void sendCateringRequest (struct socket_st* sock, byte_t *file_path)
{
    byte_t data[strlen(file_path) + 1];
    memcpy(data, file_path, strlen(file_path) + 1);
    struct package* cat_pack = buildPackage(strlen(file_path) + 1, 0, CODE_CAT, data);
    sendPackage(sock, cat_pack);
    destroyPackage(cat_pack);
} 


// Função que realiza o envio de um pacote de VER
void sendVerifyRequest (struct socket_st* sock, byte_t *file_path)
{
    byte_t data[strlen(file_path) + 1];
    memcpy(data, file_path, strlen(file_path) + 1);
    struct package* ver_pack = buildPackage(strlen(file_path) + 1, 0, CODE_VER, data);
    sendPackage(sock, ver_pack);
    destroyPackage(ver_pack);
}


// Função que realiza o envio de um pacote de OK_CK
void sendOKandCK (struct socket_st* sock, size_t ck_sum)
{
    byte_t data[sizeof(size_t)];
    memcpy(data, &ck_sum, sizeof(size_t));
    struct package* ok_ck_pack = buildPackage(sizeof(size_t), 0, CODE_OK_CK, data);
    sendPackage(sock, ok_ck_pack);
    destroyPackage(ok_ck_pack);
}


// Função que realiza o envio de um pacote de OK_LEN
void sendOKandLEN (struct socket_st* sock, size_t length)
{
    byte_t data[sizeof(size_t)];
    memcpy(data, &length, sizeof(size_t));
    struct package* ok_len_pack = buildPackage(sizeof(size_t), 0, CODE_OK_LEN, data);
    sendPackage(sock, ok_len_pack);
    destroyPackage(ok_len_pack);
}


// Função que realiza o envio de um pacote de LEN
void sendFileLength (struct socket_st* sock, size_t length)
{
    byte_t data[sizeof(size_t)];
    memcpy(data, &length, sizeof(size_t));
    struct package* len_pack = buildPackage(sizeof(size_t), 0, CODE_LEN, data);
    sendPackage(sock, len_pack);
    destroyPackage(len_pack);
}


// Função que realiza o envio de um pacote de DATA
void sendData (struct socket_st* sock, byte_t len, byte_t seq, byte_t *data)
{
    struct package* data_pack = buildPackage(len, seq, CODE_DATA, data);
    sendPackage(sock, data_pack);
    destroyPackage(data_pack);
}


// Função que realiza o envio de um pacote de DATA_END
void sendDataEnd (struct socket_st* sock)
{
    byte_t data[1] = {0};
    struct package* data_end_pack = buildPackage(1, 0, CODE_DATA_END, data);
    sendPackage(sock, data_end_pack);
    destroyPackage(data_end_pack);
}


// Função que realiza o envio de um pacote de Erro
void sendError (struct socket_st* sock, byte_t error_number)
{
    byte_t data[1] = {error_number};
    struct package* error_pack = buildPackage(1, 0, CODE_ERROR, data);
    sendPackage(sock, error_pack);
    destroyPackage(error_pack);
}


// Função que calcula o timeout 
bool timeout(time_t begin) 
{
    time_t now = time(NULL);

    return (now - begin) >= TIMEOUT;
}


// Função que imprime um código de erro
void printError(byte_t error, char* org)
{
    if (error == ERROR_OPN)
        fprintf(stderr, "ERROR: não foi possível abrir o arquivo no %s.\n", org);
    else if (error == ERROR_MEM)
        fprintf(stderr, "ERROR: não existe espaço suficiente para o arquivo na memória do %s.\n.", org);
    else if (error == ERROR_FND)
        fprintf(stderr, "ERROR: arquivo não encontrado no %s.\n", org);
    else if (error == ERROR_CKS)
        fprintf(stderr, "ERROR: não foi possível calcular o checksum no %s.\n", org);
    else if (error == ERROR_SKT)
        fprintf(stderr, "ERROR: %s.\n", org);
}


// Função que verifica se existe memória disponível no disco
bool verifyMemoryAvailable(size_t length)
{
    FILE *file = fopen("/proc/meminfo", "r");
    if (file == NULL)
        return false;

    char line[256];
    size_t memAvailable = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line, "MemAvailable: %zu", &memAvailable);
            memAvailable *= 1024;
            break;
        }
    }
    fclose(file);

    if (memAvailable == 0)
        return false;

    return length <= memAvailable;
}


// Função que retorna o nome da primeira interface válida de rede
char* getInterfaceName()
{
    struct ifaddrs *ifaddr, *ifa;

    // Obtém a lista de interfaces de rede
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(1);
    }

    // Percorre a lista de interfaces
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        // Ignora a interface de loopback (lo) e interfaces sem endereço
        if (ifa->ifa_addr == NULL || strcmp(ifa->ifa_name, "lo") == 0) {
            continue;
        }

        // Retorna o nome da primeira interface válida (não "lo")
        char *interface_name = strdup(ifa->ifa_name);
        freeifaddrs(ifaddr);
        return interface_name;
    }

    // Caso não haja interfaces válidas, retorna NULL
    freeifaddrs(ifaddr);
    return NULL;
}