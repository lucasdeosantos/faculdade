#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smpl.h"

#define start 1
#define msg 2

typedef struct {
    int id;         // identificador de facility do SMPL
    int lider;      // id do maior líder visto até agora
    int candidato;  // 1 se for candidato, 0 se não for
} Processo;

typedef struct {
    int candidato;  // o id do processo que iniciou está mensagem de eleição
} Mensagem;

Processo *processo;
Mensagem *buffer;

static int N;
int mensagens;

// função auxiliar para enviar mensagem ao próximo vizinho no anel
void enviar(int origem, int candidato) {
    int destino = (origem + 1) % N; // destino é o próximo vizinho no anel

    buffer[destino].candidato = candidato;

    mensagens++;
    schedule(msg, 1.0, destino); 
}

int main(int argc, char *argv[]) {
    static int token, 
    event, 
    maxTempoSimulacao = 120;
    
    static char fa_name[5];

    if (argc != 2) {
        printf("Uso: ./chang_roberts <N>\n");
        return 1;
    }

    N = atoi(argv[1]);

    printf("===============================================================\n");
    printf("               Sistemas Distribuídos Prof. Elias               \n");
    printf("               Lucas Emanuel de Oliveira Santos                \n");
    printf("                      Vitor Lorenzo Cumim                      \n");
    printf("                   LOG do Trabalho Prático 1                   \n");
    printf("                   Algoritmo de Chang-Roberts                  \n");
    printf("       Este programa foi executado para: N=%d processos.       \n", N);
    printf("===============================================================\n");

    // executa os três cenários requisitados
    for(int c = 1; c <= 3; c++) {
        smpl(0, "Chang-Roberts");
        reset();
        stream(1);
        mensagens = 0;

        processo = (Processo*)malloc(sizeof(Processo) * N);
        buffer = (Mensagem*)malloc(sizeof(Mensagem) * N);

        for (int i = 0; i < N; i++) {
            memset(fa_name,'\0', 5);
            sprintf(fa_name, "%d", i);
            processo[i].id = facility(fa_name, 1);
            processo[i].lider = -1;
            processo[i].candidato = 0;
        }

        printf("\n===============================================================\n");
        if (c == 1) {
            printf(" CENÁRIO 1: Apenas o processo 0 é candidato\n");
            processo[0].candidato = 1;
            processo[0].lider = 0;
            schedule(start, 1.0, 0);
        }
        else if (c == 2) {
            printf(" CENÁRIO 2: Candidatos escolhidos aleatoriamente\n");
            int qtd_candidatos = 0;
            for (int i = 0; i < N; i++) {
                if (uniform(0.0, 100.0) < 50.0) {
                    processo[i].candidato = 1;
                    processo[i].lider = i;
                    schedule(start, 1.0, i);
                    qtd_candidatos++;
                }
            }
            if (qtd_candidatos == 0) {
                printf(" Nenhum candidato sorteado! Forçando Processo 0 como candidato.\n");
                processo[0].candidato = 1;
                processo[0].lider = 0;
                schedule(start, 1.0, 0);
            }
        }
        else {
            printf(" CENÁRIO 3: Todos os processos são candidatos\n");
            for (int i = 0; i < N; i++) {
                processo[i].candidato = 1;
                processo[i].lider = i;
                schedule(start, 1.0, i);
            }
        }
        printf("===============================================================\n");

        int eleicao_concluida = 0;
        while (time() < maxTempoSimulacao) {

            cause(&event, &token);
            switch (event) {
                case start: // inicia o processo de eleição enviando o seu id como um dos candidatos
                    if (processo[token].candidato) {
                        printf("Processo %d: Sou candidato! Início a eleição enviando meu id.\n", token);
                        enviar(token, token);
                    }
                    break;
                case msg: { // recebeu uma mensagem no anel
                    Mensagem mensagem = buffer[token];

                    // se a mensagem com o meu próprio id voltou para mim, sou o líder!
                    if (mensagem.candidato == token) {
                        printf("Processo %d: Recebi minha própria mensagem de volta no tempo %4.1f!\n", token, time());
                        printf("===============================================================\n");
                        printf("LÍDER ELEITO: Processo %d\n", token);
                        printf("Quantidade de Mensagens: %d\n", mensagens);
                        printf("Tempo Total de Simulação = %4.1f\n", time());
                        printf("===============================================================\n");
                        
                        eleicao_concluida = 1;
                        break;
                    }

                    // se o id recebido é maior do que o maior líder que conheço, repasso a mensagem para o próximo processo do anel
                    if (mensagem.candidato > processo[token].lider) {
                        processo[token].lider = mensagem.candidato;
                        printf("Processo %d: Recebi candidato=%d no tempo %4.1f. Maior que meu líder conhecido. Repassando...\n", 
                            token, mensagem.candidato, time());
                        enviar(token, mensagem.candidato);
                    }
                    // se o id recebido for menor ou igual ao meu líder conhecido, a mensagem é descartada
                    else {
                        printf("Processo %d: Recebi candidato=%d no tempo %4.1f. Menor ou igual ao meu líder conhecido. Descartada.\n", 
                            token, mensagem.candidato, time());
                    }
                    break;
                }
            }
            if (eleicao_concluida) break;
        }

        if (!eleicao_concluida) {
            printf("\n===============================================================\n");
            printf("Tempo esgotado sem consenso. A simulação foi abortada.\n");
            printf("===============================================================\n");
        }

        free(processo);
        free(buffer);
    }

    return 0;
}