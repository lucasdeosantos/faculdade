#include <stdio.h>
#include "ast.h"
#include "parser.tab.h"

extern ASTNode *program_root;
extern int semantic_errors_count;

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_pascal>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    extern FILE *yyin;
    yyin = f;
 
    int parse_result = yyparse();

    fclose(f);

    if (parse_result == 0 && semantic_errors_count == 0) {
        printf("Compilação bem-sucedida!\n");
        printf("\n--- AST Gerada ---\n");
        if (program_root) print_ast(program_root);
        printf("------------------\n");
        return 0;
    }
    else {
        if (parse_result != 0)
            fprintf(stderr, "Erro de sintaxe durante a compilação.\n");

        if (semantic_errors_count > 0)
            fprintf(stderr, "Erro(s) semântico(s) encontrado(s) durante a compilação. Total: %d\n", semantic_errors_count);
        
        printf("Compilação falhou.\n");
        return 1;
    }
}