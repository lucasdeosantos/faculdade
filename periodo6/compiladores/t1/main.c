#include <stdio.h>
#include "ast.h"
#include "symbols_table.h"
#include "codegen.h"
#include "parser.tab.h"

extern ASTNode *program_root;
extern int semantic_errors_count;
extern int yyparse();
extern FILE *yyin;

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_pascal>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Erro ao abrir arquivo");
        return 1;
    }

    yyin = f;

    printf("Iniciando análise...\n");
    int parse_result = yyparse();

    fclose(f);

    if (parse_result == 0 && semantic_errors_count == 0) {
        char *output_filename = "output.ll"; // lembrar de mudar para argv[1].ll
        printf("Análise sintática e semântica bem-sucedidas!\n");
/*
        printf("\n--- AST Gerada ---\n");
        if (program_root) print_ast(program_root);
        printf("------------------\n\n");
*/
        printf("Iniciando geração de código LLVM...\n");
        if (program_root) {
            codegen(program_root, output_filename);
        } 
        else {
            fprintf(stderr, "Erro: Não há AST para gerar código.\n");
            return 1;
        }
        printf("Geração de código LLVM concluída. Verifique 'output.ll'.\n", argv[1]);
        
        if (program_root) free_ast(program_root);
        free_symbols();
        return 0;
    } 
    else {
        if (parse_result != 0)
            fprintf(stderr, "Erro de sintaxe durante a compilação.\n");

        if (semantic_errors_count > 0)
            fprintf(stderr, "Erro(s) semântico(s) encontrado(s) durante a compilação. Total: %d\n", semantic_errors_count);
        
        printf("Compilação falhou.\n");
        
        if (program_root) free_ast(program_root);
        free_symbols();
        return 1;
    }
}