#ifndef SYMBOLS_TABLE_H
#define SYMBOLS_TABLE_H

#include "ast.h"

typedef enum {
    SYMBOL_TYPE_VAR,
    SYMBOL_TYPE_FUNCTION,
    SYMBOL_TYPE_PROCEDURE,
    SYMBOL_TYPE_PROGRAM
} SymbolCategory;

typedef struct Symbol {
    char *name;
    SymbolCategory category;
    ASTDataType data_type;
    int is_reference;
    struct Symbol *next;
} Symbol;

void insert_symbol(char *name, SymbolCategory category, ASTDataType data_type, int is_reference);
Symbol *lookup_symbol(const char *name);
void free_symbols();

#endif // SYMBOLS_TABLE_H