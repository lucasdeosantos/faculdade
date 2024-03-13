#ifndef VINAPP_H
#define VINAPP_H

#include "diretorio.h"

#define BUFFER_SIZE 1024

int inserirSempre(FILE *vpp, struct diretorio *dir, char *arquivo);

int inserirMaisRecente(FILE *vpp, struct diretorio *dir, char *arquivo);

int inserir(int (*funcao) (FILE *, struct diretorio *, char *), FILE *vpp, struct diretorio *dir, char *arquivo);

int remover(FILE *vpp, struct diretorio *dir, char *arquivo);

int mover(FILE *vpp, struct diretorio *dir, char *target, char *arquivo);

int extrair(FILE *vpp, struct diretorio *dir, char *arquivo);

void imprimirMensagemAjuda();

#endif // VINAPP_H
