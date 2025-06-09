%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbols_table.h"

extern int yylex();
extern void yyerror(const char *s);
extern FILE *yyin;

ASTNode *program_root;

%}

%union {
    double num;
    char *id;
    int op;
    ASTNode *node;
    ASTDataType type;
}

%token PROGRAM VAR INTEIRO REAL FUNCTION PROCEDURE BEGIN_TOKEN END IF THEN ELSE WHILE DO READ WRITE WRITELN
%token OPERADOR_ATRIBUICAO
%token PONTO_FINAL DOIS_PONTOS PONTO_VIRGULA VIRGULA ABRE_PARENTESES FECHA_PARENTESES
%token <id> ID
%token <num> NUM
%token <op> OPERADOR_RELACIONAL OPERADOR_MULTIPLICATIVO MAIS MENOS OR

%type <node> PROGRAMA LISTA_DE_IDENTIFICADORES DECLARACOES DECLARACOES_DE_SUBPROGRAMAS
%type <node> DECLARACAO_DE_SUBPROGRAMA CABECALHO_DE_SUBPROGRAMA ARGUMENTOS LISTA_DE_PARAMETROS
%type <node> ENUNCIADO_COMPOSTO ENUNCIADOS_OPCIONAIS LISTA_DE_ENUNCIADOS
%type <node> ENUNCIADO VARIAVEL CHAMADA_DE_PROCEDIMENTO
%type <node> LISTA_DE_EXPRESSOES EXPRESSAO EXPRESSAO_SIMPLES TERMO FATOR SINAL
%type <type> TIPO

%left OR
%left MAIS MENOS
%left OPERADOR_MULTIPLICATIVO

%%

PROGRAMA:
    PROGRAM ID ABRE_PARENTESES LISTA_DE_IDENTIFICADORES FECHA_PARENTESES PONTO_VIRGULA
    DECLARACOES
    DECLARACOES_DE_SUBPROGRAMAS
    ENUNCIADO_COMPOSTO PONTO_FINAL
    {
        insert_symbol($2, SYMBOL_TYPE_PROGRAM, TYPE_NONE, 0);
        program_root = create_node(AST_PROG, $7, $8);
        program_root->next = $9;
    }
;

LISTA_DE_IDENTIFICADORES:
    ID { $$ = create_var($1); }
    | LISTA_DE_IDENTIFICADORES VIRGULA ID {
        ASTNode *last = $1;
        while (last->next) last = last->next;
        last->next = create_var($3);
        $$ = $1;
    }
;

DECLARACOES:
    VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO PONTO_VIRGULA DECLARACOES {
        ASTNode *current_id = $2;
        while (current_id) {
            current_id->data_type = $4; // $4 contains the ASTDataType (TYPE_INTEGER or TYPE_REAL)

            insert_symbol(current_id->id, SYMBOL_TYPE_VAR, $4, 0);
            current_id = current_id->next;
        }
        $$ = create_node(AST_DECLARATIONS, $2, $6);
    }
    | /* vazio */ { $$ = NULL; }
;

TIPO:
    INTEIRO { $$ = TYPE_INTEGER; }
    | REAL { $$ = TYPE_REAL; }
;

DECLARACOES_DE_SUBPROGRAMAS:
    DECLARACOES_DE_SUBPROGRAMAS DECLARACAO_DE_SUBPROGRAMA PONTO_VIRGULA {
        ASTNode *last = $1;
        if (last) {
            while (last->next) last = last->next;
            last->next = $2;
            $$ = $1;
        } else {
            $$ = $2;
        }
    }
    | /* vazio */ { $$ = NULL; }
;

DECLARACAO_DE_SUBPROGRAMA:
    CABECALHO_DE_SUBPROGRAMA DECLARACOES ENUNCIADO_COMPOSTO {
        if ($1->type == AST_FUNCTION) {
            $$ = create_node(AST_FUNCTION, $1, $3);
            $$->left->left = $2; // declarações dentro do header
        } else {
            $$ = create_node(AST_PROCEDURE, $1, $3);
            $$->left->left = $2;
        }
    }
;

CABECALHO_DE_SUBPROGRAMA:
    FUNCTION ID ARGUMENTOS DOIS_PONTOS TIPO PONTO_VIRGULA
    {
        insert_symbol($2, SYMBOL_TYPE_FUNCTION, $5, 0);
        $$ = create_node(AST_FUNCTION, create_var($2), $3);
        $$->data_type = $5;
    }
    | PROCEDURE ID ARGUMENTOS PONTO_VIRGULA
    {
        insert_symbol($2, SYMBOL_TYPE_PROCEDURE, TYPE_NONE, 0);
        $$ = create_node(AST_PROCEDURE, create_var($2), $3);
    }
;


ARGUMENTOS:
    ABRE_PARENTESES LISTA_DE_PARAMETROS FECHA_PARENTESES { $$ = $2; }
    | /* vazio */ { $$ = NULL; }
;

LISTA_DE_PARAMETROS:
    LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {
        ASTNode *current_id = $1;
        ASTNode *head = NULL, *tail = NULL;
        while (current_id) {
            // When creating param_node, create_param already sets data_type
            ASTNode *param_node = create_param(current_id->id, $3, 0);
            if (!head) head = tail = param_node;
            else { tail->next = param_node; tail = param_node; }
            insert_symbol(current_id->id, SYMBOL_TYPE_VAR, $3, 0);
            current_id = current_id->next;
        }
        $$ = head;
    }
    | VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {
        ASTNode *current_id = $2;
        ASTNode *head = NULL, *tail = NULL;
        while (current_id) {
            // When creating param_node, create_param already sets data_type
            ASTNode *param_node = create_param(current_id->id, $4, 1);
            if (!head) head = tail = param_node;
            else { tail->next = param_node; tail = param_node; }
            insert_symbol(current_id->id, SYMBOL_TYPE_VAR, $4, 1);
            current_id = current_id->next;
        }
        $$ = head;
    }
    | LISTA_DE_PARAMETROS PONTO_VIRGULA LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {
        ASTNode *current_id = $3;
        ASTNode *tail = $1;
        while (tail->next) tail = tail->next;
        while (current_id) {
            // When creating param_node, create_param already sets data_type
            ASTNode *param_node = create_param(current_id->id, $5, 0);
            tail->next = param_node;
            tail = param_node;
            insert_symbol(current_id->id, SYMBOL_TYPE_VAR, $5, 0);
            current_id = current_id->next;
        }
        $$ = $1;
    }
    | LISTA_DE_PARAMETROS PONTO_VIRGULA VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {
        ASTNode *current_id = $4;
        ASTNode *tail = $1;
        while (tail->next) tail = tail->next;
        while (current_id) {
            // When creating param_node, create_param already sets data_type
            ASTNode *param_node = create_param(current_id->id, $6, 1);
            tail->next = param_node;
            tail = param_node;
            insert_symbol(current_id->id, SYMBOL_TYPE_VAR, $6, 1);
            current_id = current_id->next;
        }
        $$ = $1;
    }
;

ENUNCIADO_COMPOSTO:
    BEGIN_TOKEN ENUNCIADOS_OPCIONAIS END {
        $$ = $2;
    }
;

ENUNCIADOS_OPCIONAIS:
    LISTA_DE_ENUNCIADOS { $$ = $1; }
    | /* vazio */ { $$ = NULL; }
;

LISTA_DE_ENUNCIADOS:
    ENUNCIADO { $$ = $1; } 
    | LISTA_DE_ENUNCIADOS PONTO_VIRGULA ENUNCIADO {
        ASTNode *last = $1;
        while (last->next) last = last->next;
        last->next = $3;
        $$ = $1;
    }
    | LISTA_DE_ENUNCIADOS PONTO_VIRGULA {
        $$ = $1;
    }
;

ENUNCIADO:
    VARIAVEL OPERADOR_ATRIBUICAO EXPRESSAO { $$ = create_binop('=', $1, $3); }
    | CHAMADA_DE_PROCEDIMENTO { $$ = $1; }
    | ENUNCIADO_COMPOSTO { $$ = $1; }
    | IF EXPRESSAO THEN ENUNCIADO ELSE ENUNCIADO { $$ = create_if($2, $4, $6); }
    | WHILE EXPRESSAO DO ENUNCIADO { $$ = create_while($2, $4); }
    | READ ABRE_PARENTESES LISTA_DE_IDENTIFICADORES FECHA_PARENTESES { $$ = create_node(AST_READ, $3, NULL); }
    | WRITE ABRE_PARENTESES EXPRESSAO FECHA_PARENTESES { $$ = create_write($3); }
    | WRITELN ABRE_PARENTESES FECHA_PARENTESES { $$ = create_node(AST_WRITELN, NULL, NULL); } // Handles writeln(); for a simple newline
;

VARIAVEL:
    ID { $$ = create_var($1); }
;

CHAMADA_DE_PROCEDIMENTO:
    ID { $$ = create_call($1, NULL); }
    | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES { $$ = create_call($1, $3); }
;

LISTA_DE_EXPRESSOES:
    EXPRESSAO { $$ = $1; }
    | LISTA_DE_EXPRESSOES VIRGULA EXPRESSAO {
        ASTNode *last = $1;
        while (last->next) last = last->next;
        last->next = $3;
        $$ = $1;
    }
;

EXPRESSAO:
    EXPRESSAO_SIMPLES { $$ = $1; }
    | EXPRESSAO_SIMPLES OPERADOR_RELACIONAL EXPRESSAO_SIMPLES { $$ = create_binop($2, $1, $3); }
;

EXPRESSAO_SIMPLES:
    TERMO { $$ = $1; }
    | SINAL TERMO { $$ = create_unop($1->op, $2); }
    | EXPRESSAO_SIMPLES MAIS EXPRESSAO_SIMPLES { $$ = create_binop('+', $1, $3); }
    | EXPRESSAO_SIMPLES MENOS EXPRESSAO_SIMPLES { $$ = create_binop('-', $1, $3); }
    | EXPRESSAO_SIMPLES OR EXPRESSAO_SIMPLES { $$ = create_binop('|', $1, $3); }
;

TERMO:
    FATOR { $$ = $1; }
    | TERMO OPERADOR_MULTIPLICATIVO FATOR { $$ = create_binop($2, $1, $3); }
;

FATOR:
    ID { $$ = create_var($1); } // Note: Type for FATOR AST node (if it's an ID) will be set during semantic analysis
    | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES { $$ = create_call($1, $3); }
    | NUM { $$ = create_num($1); }
    | ABRE_PARENTESES EXPRESSAO FECHA_PARENTESES { $$ = $2; }
;

SINAL:
    MAIS { $$ = create_node(AST_UNOP, NULL, NULL); $$->op = '+'; }
    | MENOS { $$ = create_node(AST_UNOP, NULL, NULL); $$->op = '-'; }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro de sintaxe: %s\n", s);
}