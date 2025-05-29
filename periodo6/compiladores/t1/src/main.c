#include <stdio.h>
#include <stdlib.h>

extern int yyparse();
extern void free_all_scopes();

void yyerror(const char *s) {
    fprintf(stderr, "Erro: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_pascal>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir arquivo", argv[0]);
        return 1;
    }

    extern FILE *yyin;
    yyin = file;

    if (yyparse() == 0)
        printf("Análise sintática concluída com sucesso!\n");
    else
        printf("Erros encontrados durante a análise.\n");

    fclose(file);
    free_all_scopes();
    return 0;
}
