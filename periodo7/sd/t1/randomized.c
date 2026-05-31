#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smpl.h"

#define round 1
#define msg 2

typedef struct {
    int id;          // identificador de facility do SMPL
    int ativo;       // 1 se continua na disputa, 0 se virou repetidor
    int bit;         // bit sorteado na rodada
    int candidatos;  // quantos concorrentes com bit=1 passaram por mim nesta rodada
    int rodada;      // controle de qual rodada o processo está executando
} Processo;

typedef struct {
    int id;          // id do processo de origem da mensagem
    int bit;         // o bit sorteado pelo processo de origem
} Mensagem;

Processo *processo;
Mensagem **buffer;

static int N;
int mensagens = 0;

// função auxiliar para enviar mensagem ao próximo vizinho no anel
void enviar(int origem, int id, int bit) {
    int destino = (origem + 1) % N; // destino é o próximo vizinho no anel
    
    buffer[destino][id].id = id;
    buffer[destino][id].bit = bit;
    mensagens++;
    
    int token = (destino * 1000) + id; // cálculo para enviar tanto o destino como o id em um único token
    schedule(msg, 1.0, token); 
}

int main(int argc, char *argv[]) {
    static int event, 
    token,
    maxTempoSimulacao = 120;

    static char fa_name[5];

    if (argc != 2) {
        printf("Uso: ./randomized <N>\n");
        return 1;
    }

    N = atoi(argv[1]);

    smpl(0, "Randomized Leader Election");
    reset();
    stream(1);

    processo = (Processo*)malloc(sizeof(Processo) * N);
    buffer = (Mensagem**)malloc(sizeof(Mensagem*) * N);
    for (int i = 0; i < N; i++) 
        buffer[i] = (Mensagem*)malloc(sizeof(Mensagem) * N);

    for (int i = 0; i < N; i++) {
        memset(fa_name,'\0', 5);
        sprintf(fa_name, "%d", i);
        processo[i].id = facility(fa_name, 1);
        processo[i].ativo = 1; 
        processo[i].bit = 0;
        processo[i].candidatos = 0;
        processo[i].rodada = 1;
    }

    printf("===============================================================\n");
    printf("               Sistemas Distribuídos Prof. Elias               \n");
    printf("               Lucas Emanuel de Oliveira Santos                \n");
    printf("                      Vitor Lorenzo Cumim                      \n");
    printf("                   LOG do Trabalho Prático 1                   \n");
    printf("               Algoritmo de Eleição Aleatorizada               \n");
    printf("       Este programa foi executado para: N=%d processos.       \n", N);
    printf("===============================================================\n\n");

    for (int i = 0; i < N; i++) 
        schedule(round, 0.0, i);

    int liderEleito = 0;
    while (time() < maxTempoSimulacao && !liderEleito) {

        cause(&event, &token);
        switch (event) {
            case round: { // inicia uma rodada como um dos candidatos sorteando o seu bit e enviando a mensagem para o anel 
                if (processo[token].ativo) {
                    processo[token].bit = (uniform(0.0, 100.0) < 50.0) ? 1 : 0;
                    processo[token].candidatos = 0;
                    printf("Processo %d: Sou candidato na rodada %d e sorteei o bit %d no tempo %4.1f. Enviando mensagem...\n", 
                           token, processo[token].rodada, processo[token].bit, time());
                    
                    enviar(token, token, processo[token].bit);
                }
                break;
            }
            case msg: { // recebeu uma mensagem no anel
                int destino = token / 1000;
                int origem = token % 1000;
                Mensagem mensagem = buffer[destino][origem];

                // cenário 1: a mensagem completou a volta no anel e retornou ao seu criador
                if (mensagem.id == destino) {
                    if (processo[destino].candidatos == 0) {
                        if (processo[destino].bit == 1) {
                            // fui o único a tirar 1, sou o líder!
                            printf("Processo %d: Recebi minha própria mensagem de volta no tempo %4.1f! Fui o único a tirar 1.\n", destino, time());
                            printf("\n===============================================================\n");
                            printf("LÍDER ELEITO: Processo %d\n", destino);
                            printf("Decidido na Rodada: %d\n", processo[destino].rodada);
                            printf("Quantidade de Mensagens: %d\n", mensagens);
                            printf("Tempo Total de Simulação: %4.1f\n", time());
                            printf("===============================================================\n");
                            
                            liderEleito = 1;
                        } 
                        else {
                            // tirei 0 e não vi ninguém tirar 1, então todos os processos tiraram zero e a rodada deve reiniciar
                            printf("Processo %d: Recebi minha própria mensagem de volta no tempo %4.1f. Todos tiraram 0. Tentando de novo...\n", 
                                   destino, time());
                            processo[destino].ativo = 1;
                            schedule(round, 1.0, destino);
                        }
                    } 
                    else {
                        // alguém tirou 1, vamos ver se vou para a próxima rodada
                        if (processo[destino].bit == 1) {
                            // tirei 1, vou para a próxima rodada
                            printf("Processo %d: Recebi minha própria mensagem de volta no tempo %4.1f. Avançando para rodada %d...\n", 
                                   destino, time(), processo[destino].rodada + 1);
                            processo[destino].ativo = 1;
                            processo[destino].rodada++;
                            schedule(round, 1.0, destino);
                        } 
                        else {
                            // tirei 0, fui eliminado
                            printf("Processo %d: Recebi minha própria mensagem de volta no tempo %4.1f. Fui eliminado.\n", destino, time());
                            processo[destino].ativo = 0;
                        }
                    }
                } 
                // cenário 2: a mensagem está apenas passando pelo processo
                else {
                    if (mensagem.bit == 1)
                        processo[destino].candidatos++;
                    
                    printf("Processo %d: Recebi mensagem de %d no tempo %4.1f. Repassando...\n", destino, mensagem.id, mensagem.bit, time());
                    enviar(destino, mensagem.id, mensagem.bit);
                }
                break;
            }
        }
    }

    if (!liderEleito) {
        printf("\n===============================================================\n");
        printf("Tempo esgotado sem consenso. A simulação foi abortada.\n");
        printf("===============================================================\n");
    }

    for (int i = 0; i < N; i++) 
        free(buffer[i]);
    free(buffer);
    free(processo);
    
    return 0;
}