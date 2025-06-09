#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols_table.h"

static Symbol *symbol_table = NULL;
int semantic_errors_count = 0;

void insert_symbol(char *name, SymbolCategory category, ASTDataType data_type, int is_reference) {
    if (lookup_symbol(name)) {
        fprintf(stderr, "Erro: símbolo '%s' já declarado.\n", name);
        semantic_errors_count++;
        return;
    }

    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    if (new_symbol == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória para o símbolo '%s'.\n", name);
        exit(EXIT_FAILURE);
    }

    new_symbol->name = strdup(name);
    if (new_symbol->name == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória para o nome do símbolo '%s'.\n", name);
        exit(EXIT_FAILURE);
    }

    new_symbol->category = category;
    new_symbol->data_type = data_type;
    new_symbol->is_reference = is_reference;
    new_symbol->next = symbol_table;
    symbol_table = new_symbol;
}

Symbol *lookup_symbol(const char *name) {
    Symbol *current = symbol_table;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void free_symbols() {
    Symbol *current = symbol_table;
    while (current) {
        Symbol *next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
    symbol_table = NULL;
}