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
    int num;
    char *id;
    ASTNode *node;
    ASTDataType type;
}

%token PROGRAM VAR INTEIRO REAL FUNCTION PROCEDURE BEGIN_TOKEN END IF THEN ELSE WHILE DO READ WRITE WRITELN
%token ATRIBUICAO MAIS MENOS OR AND MULTIPLICACAO DIVISAO DIV MOD
%token PONTO_FINAL DOIS_PONTOS PONTO_VIRGULA VIRGULA ABRE_PARENTESES FECHA_PARENTESES
%token EQ NE LT LE GT GE

%token <id> ID
%token <num> NUM

%type <node> PROGRAMA LISTA_DE_IDENTIFICADORES DECLARACOES BLOCO_SUBPROGRAMA DECLARACOES_DE_SUBPROGRAMAS
%type <node> DECLARACAO_DE_SUBPROGRAMA CABECALHO_DE_SUBPROGRAMA ARGUMENTOS LISTA_DE_PARAMETROS
%type <node> ENUNCIADO_COMPOSTO ENUNCIADOS_OPCIONAIS LISTA_DE_ENUNCIADOS
%type <node> ENUNCIADO VARIAVEL CHAMADA_DE_PROCEDIMENTO
%type <node> LISTA_DE_EXPRESSOES EXPRESSAO EXPRESSAO_SIMPLES TERMO FATOR SINAL
%type <type> TIPO

%left OR
%left AND
%left MAIS MENOS
%left MULTIPLICACAO DIVISAO DIV MOD
%nonassoc EQ NE LT LE GT GE

%%

PROGRAMA:
    PROGRAM ID ABRE_PARENTESES LISTA_DE_IDENTIFICADORES FECHA_PARENTESES PONTO_VIRGULA
    {
        enter_scope($2);
        insert_symbol($2, SYMBOL_TYPE_PROGRAM, TYPE_NONE, 0, NULL);
    }
    DECLARACOES
    DECLARACOES_DE_SUBPROGRAMAS
    ENUNCIADO_COMPOSTO PONTO_FINAL
    {
        program_root = create_program($2, $8, $9, $10);
        exit_scope();
    }
;

LISTA_DE_IDENTIFICADORES:
    ID {
        $$ = create_var($1);
    }
    | LISTA_DE_IDENTIFICADORES VIRGULA ID {
        ASTNode *current = $1;
        while (current->next)
            current = current->next;

        current->next = create_var($3);
        $$ = $1;
    }
;

DECLARACOES:
    VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO PONTO_VIRGULA DECLARACOES {
        ASTNode *current_id = $2;
        while (current_id) {
            current_id->data_type = $4;
            insert_symbol(current_id->id, SYMBOL_TYPE_VAR, $4, 0, NULL);
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
        }
        else {
            $$ = $2;
        }
    }
    | /* vazio */ { $$ = NULL; }
;

DECLARACAO_DE_SUBPROGRAMA:
    CABECALHO_DE_SUBPROGRAMA BLOCO_SUBPROGRAMA
    {
        ASTNode *subprogram_node;
        if ($1->type == AST_FUNCTION)
            subprogram_node = create_function($1->id, $1->left, $1->data_type, $2);
        else
            subprogram_node = create_procedure($1->id, $1->left, $2);

        exit_scope();
        $$ = subprogram_node;
    }
;

BLOCO_SUBPROGRAMA:
    DECLARACOES ENUNCIADO_COMPOSTO
    {
        $$ = create_function_block($1, $2);
    }
;

CABECALHO_DE_SUBPROGRAMA:
    FUNCTION ID {
        insert_symbol($2, SYMBOL_TYPE_FUNCTION, TYPE_NONE, 0, NULL);
        enter_scope($2);
        insert_symbol($2, SYMBOL_TYPE_VAR, TYPE_NONE, 0, NULL);
    } ARGUMENTOS DOIS_PONTOS TIPO PONTO_VIRGULA
    {
        ASTNode *header_node = create_node(AST_FUNCTION, NULL, NULL);
        header_node->id = strdup($2);
        header_node->left = $4;
        header_node->data_type = $6;

        Scope *previous_scope = current_scope;
        current_scope = global_scope_head;

        Symbol *global_func_sym = lookup_symbol($2); 
        if (global_func_sym && global_func_sym->category == SYMBOL_TYPE_FUNCTION) {
            global_func_sym->data_type = $6;
            global_func_sym->ast_node_def = header_node;
        } 
        else {
            fprintf(stderr, "[SEMANTIC ERROR] Função '%s' não encontrada na tabela de símbolos global para atualização de definição, ou categoria incompatível.\n", $2);
            semantic_errors_count++;
        }

        current_scope = previous_scope;

        Symbol *local_func_var_sym = lookup_symbol_in_current_scope($2);
        if (local_func_var_sym && local_func_var_sym->category == SYMBOL_TYPE_VAR)
            local_func_var_sym->data_type = $6;

        $$ = header_node;
    }
    | PROCEDURE ID {
        insert_symbol($2, SYMBOL_TYPE_PROCEDURE, TYPE_NONE, 0, NULL);
        enter_scope($2);
    } ARGUMENTOS PONTO_VIRGULA
    {
        ASTNode *header_node = create_node(AST_PROCEDURE, NULL, NULL);
        header_node->id = strdup($2);
        header_node->left = $4;
        header_node->data_type = TYPE_NONE;

        Scope *previous_scope = current_scope;
        current_scope = global_scope_head;

        Symbol *global_proc_sym = lookup_symbol($2);
        if (global_proc_sym && global_proc_sym->category == SYMBOL_TYPE_PROCEDURE) {
            global_proc_sym->ast_node_def = header_node;
        } 
        else {
            fprintf(stderr, "[SEMANTIC ERROR] Procedimento '%s' não encontrado na tabela de símbolos global para atualização de definição, ou categoria incompatível.\n", $2);
            semantic_errors_count++;
        }

        current_scope = previous_scope;

        $$ = header_node;
    }
;

ARGUMENTOS:
    ABRE_PARENTESES LISTA_DE_PARAMETROS FECHA_PARENTESES { $$ = $2; }
    | ABRE_PARENTESES FECHA_PARENTESES { $$ = NULL; }
    | /* vazio */ { $$ = NULL; }
;

LISTA_DE_PARAMETROS:
    LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {
        ASTNode *current_id = $1;
        ASTNode *head = NULL, *tail = NULL;
        while (current_id) {
            ASTNode *param_node = create_param(current_id->id, $3, 0);
            if (!head) head = tail = param_node;
            else { tail->next = param_node; tail = param_node; }
            insert_symbol(current_id->id, SYMBOL_TYPE_VAR, $3, 0, NULL);
            current_id = current_id->next;
        }
        $$ = head;
    }
    | VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {
        ASTNode *current_id = $2;
        ASTNode *head = NULL, *tail = NULL;
        while (current_id) {
            ASTNode *param_node = create_param(current_id->id, $4, 1);
            if (!head) head = tail = param_node;
            else { tail->next = param_node; tail = param_node; }
            insert_symbol(current_id->id, SYMBOL_TYPE_VAR, $4, 1, NULL);
            current_id = current_id->next;
        }
        $$ = head;
    }
    | LISTA_DE_PARAMETROS PONTO_VIRGULA LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {
        ASTNode *current_id = $3;
        ASTNode *tail = $1;
        while (tail->next) tail = tail->next;
        while (current_id) {
            ASTNode *param_node = create_param(current_id->id, $5, 0);
            tail->next = param_node;
            tail = param_node;
            insert_symbol(current_id->id, SYMBOL_TYPE_VAR, $5, 0, NULL);
            current_id = current_id->next;
        }
        $$ = $1;
    }
    | LISTA_DE_PARAMETROS PONTO_VIRGULA VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {
        ASTNode *current_id = $4;
        ASTNode *tail = $1;
        while (tail->next) tail = tail->next;
        while (current_id) {
            ASTNode *param_node = create_param(current_id->id, $6, 1);
            tail->next = param_node;
            tail = param_node;
            insert_symbol(current_id->id, SYMBOL_TYPE_VAR, $6, 1, NULL);
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
        if (last) {
            while (last->next) last = last->next;
            last->next = $3;
            $$ = $1;
        }
        else {
            $$ = $3;
        }
    }
    | LISTA_DE_ENUNCIADOS PONTO_VIRGULA {
        $$ = $1;
    }
;

ENUNCIADO:
    VARIAVEL ATRIBUICAO EXPRESSAO {
        $$ = create_assignment($1, $3);
    }
    | CHAMADA_DE_PROCEDIMENTO {
        $$ = $1;
    }
    | ENUNCIADO_COMPOSTO {
        $$ = $1;
    }
    | IF EXPRESSAO THEN ENUNCIADO ELSE ENUNCIADO {
        $$ = create_if($2, $4, $6);
    }
    | WHILE EXPRESSAO DO ENUNCIADO {
        $$ = create_while($2, $4);
    }
    | READ ABRE_PARENTESES LISTA_DE_IDENTIFICADORES FECHA_PARENTESES {
        ASTNode *id_list = $3;
        ASTNode *current_id = id_list;
        while (current_id) {
            if (current_id->type == AST_VAR) {
                Symbol *sym = lookup_symbol(current_id->id);
                if (sym == NULL || sym->category != SYMBOL_TYPE_VAR) {
                    fprintf(stderr, "[SEMANTIC ERROR] Variavel '%s' na instrucao READ nao declarada ou nao eh uma variavel.\n", current_id->id);
                    semantic_errors_count++;
                }
                current_id->data_type = sym ? sym->data_type : TYPE_NONE;
            }
            current_id = current_id->next;
        }
        $$ = create_read(id_list);
    }
    | WRITE ABRE_PARENTESES EXPRESSAO FECHA_PARENTESES {
        $$ = create_write($3);
    }
    | WRITELN ABRE_PARENTESES FECHA_PARENTESES {
        $$ = create_node(AST_WRITELN, NULL, NULL);
    }
;

VARIAVEL:
    ID {
        Symbol *sym = lookup_symbol($1);
        if (sym == NULL || (sym->category != SYMBOL_TYPE_VAR && sym->category != SYMBOL_TYPE_FUNCTION)) {
            fprintf(stderr, "[SEMANTIC ERROR] Variavel '%s' nao declarada ou uso indevido.\n", $1);
            semantic_errors_count++;
            $$ = create_var($1);
            $$->data_type = TYPE_NONE;
        } 
        else {
            $$ = create_var($1);
            $$->data_type = sym->data_type;
        }
    }
;

CHAMADA_DE_PROCEDIMENTO:
    ID {
        Symbol *sym = lookup_symbol($1);
        if (sym == NULL || (sym->category != SYMBOL_TYPE_PROCEDURE && sym->category != SYMBOL_TYPE_FUNCTION)) {
            fprintf(stderr, "[SEMANTIC ERROR] Procedimento/Funcao '%s' nao declarado ou uso indevido.\n", $1);
            semantic_errors_count++;
            $$ = create_call($1, NULL);
            $$->data_type = TYPE_NONE;
        } 
        else {
            $$ = create_call($1, NULL);
            $$->data_type = sym->data_type;
        }
    }
    | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES {
        Symbol *sym = lookup_symbol($1);
        if (sym == NULL || (sym->category != SYMBOL_TYPE_PROCEDURE && sym->category != SYMBOL_TYPE_FUNCTION)) {
            fprintf(stderr, "[SEMANTIC ERROR] Procedimento/Funcao '%s' nao declarado ou uso indevido.\n", $1);
            semantic_errors_count++;
            $$ = create_call($1, $3);
            $$->data_type = TYPE_NONE;
        } 
        else {
            $$ = create_call($1, $3);
            $$->data_type = sym->data_type;
        }
    }
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
    | EXPRESSAO_SIMPLES EQ EXPRESSAO_SIMPLES { $$ = create_binop(OP_EQ, $1, $3); }
    | EXPRESSAO_SIMPLES NE EXPRESSAO_SIMPLES { $$ = create_binop(OP_NE, $1, $3); }
    | EXPRESSAO_SIMPLES LT EXPRESSAO_SIMPLES { $$ = create_binop(OP_LT, $1, $3); }
    | EXPRESSAO_SIMPLES LE EXPRESSAO_SIMPLES { $$ = create_binop(OP_LE, $1, $3); }
    | EXPRESSAO_SIMPLES GT EXPRESSAO_SIMPLES { $$ = create_binop(OP_GT, $1, $3); }
    | EXPRESSAO_SIMPLES GE EXPRESSAO_SIMPLES { $$ = create_binop(OP_GE, $1, $3); }
;

EXPRESSAO_SIMPLES:
    TERMO { $$ = $1; }
    | SINAL TERMO {
        $$ = create_unop($1->op, $2);
        free($1);
    }
    | EXPRESSAO_SIMPLES MAIS TERMO { $$ = create_binop(OP_PLUS, $1, $3); }
    | EXPRESSAO_SIMPLES MENOS TERMO { $$ = create_binop(OP_MINUS, $1, $3); }
    | EXPRESSAO_SIMPLES OR TERMO { $$ = create_binop(OP_OR, $1, $3); }
;

TERMO:
    FATOR { $$ = $1; }
    | TERMO MULTIPLICACAO FATOR { $$ = create_binop(OP_MUL, $1, $3); }
    | TERMO DIVISAO FATOR { $$ = create_binop(OP_DIV, $1, $3); }
    | TERMO DIV FATOR { $$ = create_binop(OP_DIV_INT, $1, $3); }
    | TERMO MOD FATOR { $$ = create_binop(OP_MOD, $1, $3); }
    | TERMO AND FATOR { $$ = create_binop(OP_AND, $1, $3); }
;

FATOR:
    ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES {
        Symbol *sym = NULL;
        if (current_scope != NULL && strcmp(current_scope->name, $1) == 0)
            if (current_scope->parent != NULL)
                sym = lookup_symbol_in_scope(current_scope->parent->name, $1);
        
        if (sym == NULL)
            sym = lookup_symbol($1);

        if (sym == NULL || sym->category != SYMBOL_TYPE_FUNCTION) {
            fprintf(stderr, "[SEMANTIC ERROR] Funcao '%s' nao declarada ou uso indevido (nao eh uma funcao).\n", $1);
            semantic_errors_count++;
            $$ = create_call($1, $3);
            $$->data_type = TYPE_NONE;
        } 
        else {
            $$ = create_call($1, $3);
            $$->data_type = sym->data_type;
        }
    }
    | ID {
        Symbol *sym = lookup_symbol($1);

        if (sym == NULL) {
            fprintf(stderr, "[SEMANTIC ERROR] Identificador '%s' nao declarado.\n", $1);
            semantic_errors_count++;
            $$ = create_var($1);
            $$->data_type = TYPE_NONE;
        }
        else if (sym->category == SYMBOL_TYPE_VAR) {
            $$ = create_var($1);
            $$->data_type = sym->data_type;
        }
        else if (sym->category == SYMBOL_TYPE_FUNCTION) {
            fprintf(stderr, "[SEMANTIC ERROR] Funcao '%s' usada sem argumentos. Funcoes devem ser chamadas com parenteses ou usadas como variavel de retorno dentro do seu proprio corpo.\n", $1);
            semantic_errors_count++;
            $$ = create_var($1);
            $$->data_type = TYPE_NONE;
        } 
        else {
            fprintf(stderr, "[SEMANTIC ERROR] Uso indevido do identificador '%s' como fator (nao eh variavel ou funcao).\n", $1);
            semantic_errors_count++;
            $$ = create_var($1);
            $$->data_type = TYPE_NONE;
        }
    }
    | NUM {
        $$ = create_num($1);
    }
    | ABRE_PARENTESES EXPRESSAO FECHA_PARENTESES {
        $$ = $2;
    }
;

SINAL:
    MAIS {
        $$ = create_node(AST_UNOP, NULL, NULL);
        $$->op = OP_UPLUS;
    }
    | MENOS {
        $$ = create_node(AST_UNOP, NULL, NULL);
        $$->op = OP_UMINUS;
    }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "[SYNTAX ERROR] %s\n", s);
}