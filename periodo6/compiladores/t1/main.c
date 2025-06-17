#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "symbols_table.h"
#include "codegen.h"
#include "parser.tab.h"

extern ASTNode *program_root;
extern int semantic_errors_count;
extern int yyparse();
extern FILE *yyin;

void print_usage(const char *prog_name) {
    printf("MiniPascal Compiler\n");
    printf("Uso: %s <arquivo_pascal>\n", prog_name);
}

int main(int argc, char **argv) {
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        fprintf(stderr, "[ERROR] Não foi possível abrir o arquivo '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    yyin = input_file;
    init_symbol_table();
    printf("[INFO] Iniciando análise do arquivo '%s'\n", argv[1]);
    int parse_result = yyparse();
    fclose(input_file);


    if (parse_result == 0 && semantic_errors_count == 0) {
        printf("[SUCCESS] Análise sintática e semântica concluídas sem erros.\n");

        char *input_filename = argv[1];
        char *dot_pos = strrchr(input_filename, '.');
        size_t filename_len = (dot_pos != NULL) ? (size_t)(dot_pos - input_filename) : strlen(input_filename);

        char *output_filename = (char *)malloc(filename_len + 4);
        if (!output_filename) {
            fprintf(stderr, "[ERROR] Falha ao alocar memória para o nome do arquivo de saída.\n");
            if (program_root) free_ast(program_root);
            free_all_scopes();
            return EXIT_FAILURE;
        }
        strncpy(output_filename, input_filename, filename_len);
        output_filename[filename_len] = '\0';
        strcat(output_filename, ".ll");

        // print_ast(program_root, 0);
        // print_symbols_table();

        printf("[INFO] Iniciando geração de código LLVM para '%s'\n", output_filename);
        if (program_root) {
            codegen(program_root, output_filename);
            printf("[SUCCESS] Geração de código LLVM concluída com sucesso!\n");
            printf("[OUTPUT] Arquivo gerado: %s\n", output_filename);
        }
        else {
            fprintf(stderr, "[ERROR] AST não gerada. Código LLVM não foi produzido.\n");
            free(output_filename);
            free_ast(program_root);
            free_all_scopes();
            return EXIT_FAILURE;
        }
        
        free(output_filename);

    } 
    else {
        if (parse_result != 0)
            fprintf(stderr, "[ERROR] Erros de sintaxe encontrados durante a análise.\n");

        if (semantic_errors_count > 0)
            fprintf(stderr, "[ERROR] Total de erros semânticos: %d\n", semantic_errors_count);

        printf("[FAILURE] A compilação foi interrompida devido a erros.\n");
    }

    free_ast(program_root);
    free_all_scopes();

    return (parse_result == 0 && semantic_errors_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
