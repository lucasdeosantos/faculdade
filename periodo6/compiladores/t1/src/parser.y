%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols_table.h"

extern int yylex();
void yyerror(const char *s);

%}

%union {
    char *id;
    VarType tipo;
    Symbol *symbolList;
}

%token PROGRAM VAR FUNCTION PROCEDURE BEGIN_TOKEN END IF THEN ELSE WHILE DO OR
%token INTEIRO REAL
%token MAIS MENOS OPERADOR_ATRIBUICAO OPERADOR_RELACIONAL OPERADOR_MULTIPLICATIVO
%token ABRE_PARENTESES FECHA_PARENTESES PONTO_VIRGULA VIRGULA DOIS_PONTOS PONTO_FINAL
%token NUM
%token <id> ID

%type <tipo> TIPO EXPRESSAO EXPRESSAO_SIMPLES TERMO FATOR VARIAVEL
%type <symbolList> LISTA_DE_IDENTIFICADORES LISTA_DE_PARAMETROS

%%

PROGRAMA:
    PROGRAM ID ABRE_PARENTESES LISTA_DE_IDENTIFICADORES FECHA_PARENTESES PONTO_VIRGULA
    { enter_scope(); }
    DECLARACOES
    DECLARACOES_DE_SUBPROGRAMAS
    ENUNCIADO_COMPOSTO
    PONTO_FINAL
    { exit_scope(); }
;

LISTA_DE_IDENTIFICADORES:
    ID
    {
        $$ = malloc(sizeof(Symbol));
        $$->name = strdup($1);
        $$->type = TYPE_VOID;
        $$->next = NULL;
    }
    | LISTA_DE_IDENTIFICADORES VIRGULA ID
    {
        Symbol *sym = malloc(sizeof(Symbol));
        sym->name = strdup($3);
        sym->type = TYPE_VOID;
        sym->next = $1;
        $$ = sym;
    }
;

DECLARACOES:
    DECLARACOES VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO PONTO_VIRGULA
    {
        Symbol *s = $3;
        while (s) {
            insert_symbol(s->name, $5);
            s = s->next;
        }
    }
    | /* empty */
;

TIPO:
    INTEIRO { $$ = TYPE_INTEGER; }
    | REAL  { $$ = TYPE_REAL; }
;

DECLARACOES_DE_SUBPROGRAMAS:
    DECLARACOES_DE_SUBPROGRAMAS DECLARACAO_DE_SUBPROGRAMA PONTO_VIRGULA
    | /* empty */
;

DECLARACAO_DE_SUBPROGRAMA:
    CABECALHO_DE_SUBPROGRAMA DECLARACOES ENUNCIADO_COMPOSTO
    { exit_scope(); }
;

CABECALHO_DE_SUBPROGRAMA:
    FUNCTION ID ARGUMENTOS DOIS_PONTOS TIPO PONTO_VIRGULA
    {
        insert_symbol($2, $5);
        enter_scope();
    }
    | PROCEDURE ID ARGUMENTOS PONTO_VIRGULA
    {
        insert_symbol($2, TYPE_VOID);
        enter_scope();
    }
;

ARGUMENTOS:
    ABRE_PARENTESES LISTA_DE_PARAMETROS FECHA_PARENTESES
    {
        Symbol *s = $2;
        while (s) {
            insert_symbol(s->name, s->type);
            s = s->next;
        }
    }
    | /* empty */
;

LISTA_DE_PARAMETROS:
    LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO
    {
        Symbol *s = $1;
        while (s) {
            s->type = $3;
            s = s->next;
        }
        $$ = $1;
    }
    | VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO
    {
        Symbol *s = $2;
        while (s) {
            s->type = $4;
            s = s->next;
        }
        $$ = $2;
    }
    | LISTA_DE_PARAMETROS PONTO_VIRGULA LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO
    {
        Symbol *s = $3;
        while (s) {
            s->type = $5;
            s = s->next;
        }
        s = $3;
        while (s->next) s = s->next;
        s->next = $1;
        $$ = $3;
    }
    | LISTA_DE_PARAMETROS PONTO_VIRGULA VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO
    {
        Symbol *s = $4;
        while (s) {
            s->type = $6;
            s = s->next;
        }
        s = $4;
        while (s->next) s = s->next;
        s->next = $1;
        $$ = $4;
    }
;

ENUNCIADO_COMPOSTO:
    BEGIN_TOKEN ENUNCIADOS_OPCIONAIS END
;

ENUNCIADOS_OPCIONAIS:
    LISTA_DE_ENUNCIADOS
    | /* empty */
;

LISTA_DE_ENUNCIADOS:
    ENUNCIADO
    | LISTA_DE_ENUNCIADOS PONTO_VIRGULA ENUNCIADO
;

ENUNCIADO:
    VARIAVEL OPERADOR_ATRIBUICAO EXPRESSAO
    | CHAMADA_DE_PROCEDIMENTO
    | ENUNCIADO_COMPOSTO
    | IF EXPRESSAO THEN ENUNCIADO ELSE ENUNCIADO
    | WHILE EXPRESSAO DO ENUNCIADO
;

VARIAVEL:
    ID
    {
        Symbol *sym = lookup_symbol($1);
        if (!sym) {
            fprintf(stderr, "Erro: variável '%s' não declarada\n", $1);
            exit(1);
        }
        $$ = sym->type;
    }
;

CHAMADA_DE_PROCEDIMENTO:
    ID
    {
        Symbol *sym = lookup_symbol($1);
        if (!sym) {
            fprintf(stderr, "Erro: procedimento '%s' não declarado\n", $1);
            exit(1);
        }
    }
    | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES
    {
        Symbol *sym = lookup_symbol($1);
        if (!sym) {
            fprintf(stderr, "Erro: procedimento/função '%s' não declarado\n", $1);
            exit(1);
        }
    }
;

LISTA_DE_EXPRESSOES:
    EXPRESSAO
    | LISTA_DE_EXPRESSOES VIRGULA EXPRESSAO
;

EXPRESSAO:
    EXPRESSAO_SIMPLES
    { $$ = $1; }
    | EXPRESSAO_SIMPLES OPERADOR_RELACIONAL EXPRESSAO_SIMPLES
    { $$ = TYPE_INTEGER; } /* Relações retornam booleano (como inteiro) */
;

EXPRESSAO_SIMPLES:
    TERMO
    { $$ = $1; }
    | SINAL TERMO
    { $$ = $2; }
    | EXPRESSAO_SIMPLES MAIS EXPRESSAO_SIMPLES
    {
        if ($1 != $3) {
            fprintf(stderr, "Erro: tipos incompatíveis na soma\n");
            exit(1);
        }
        $$ = $1;
    }
    | EXPRESSAO_SIMPLES MENOS EXPRESSAO_SIMPLES
    {
        if ($1 != $3) {
            fprintf(stderr, "Erro: tipos incompatíveis na subtração\n");
            exit(1);
        }
        $$ = $1;
    }
    | EXPRESSAO_SIMPLES OR EXPRESSAO_SIMPLES
    {
        if ($1 != TYPE_INTEGER || $3 != TYPE_INTEGER) {
            fprintf(stderr, "Erro: operador OR espera inteiros\n");
            exit(1);
        }
        $$ = TYPE_INTEGER;
    }
;

TERMO:
    FATOR
    { $$ = $1; }
    | TERMO OPERADOR_MULTIPLICATIVO FATOR
    {
        if ($1 != $3) {
            fprintf(stderr, "Erro: tipos incompatíveis na operação multiplicativa\n");
            exit(1);
        }
        $$ = $1;
    }
;

FATOR:
    ID
    {
        Symbol *sym = lookup_symbol($1);
        if (!sym) {
            fprintf(stderr, "Erro: variável '%s' não declarada\n", $1);
            exit(1);
        }
        $$ = sym->type;
    }
    | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES
    {
        Symbol *sym = lookup_symbol($1);
        if (!sym) {
            fprintf(stderr, "Erro: função '%s' não declarada\n", $1);
            exit(1);
        }
        $$ = sym->type;
    }
    | NUM
    { $$ = TYPE_INTEGER; }
    | ABRE_PARENTESES EXPRESSAO FECHA_PARENTESES
    { $$ = $2; }
;

SINAL:
    MAIS
    | MENOS
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro: %s\n", s);
}