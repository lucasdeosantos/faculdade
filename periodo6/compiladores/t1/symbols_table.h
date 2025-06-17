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
    struct ASTNode *ast_node_def;
} Symbol;

typedef struct Scope {
    int level;
    char *name;
    Symbol *head;
    struct Scope *parent;
    struct Scope *next_sibling;
    struct Scope *first_child;
} Scope;

extern Scope *current_scope;
extern Scope *global_scope_head;
extern int semantic_errors_count;

void init_symbol_table();
void enter_scope(char *scope_name);
void exit_scope();
void insert_symbol(char *name, SymbolCategory category, ASTDataType data_type, int is_reference, ASTNode *ast_node_def);
Symbol *lookup_symbol_in_current_scope(const char *name);
Symbol* lookup_symbol_in_scope(const char* scope_name, const char* symbol_name);
Symbol *lookup_symbol(const char *name);
Symbol* lookup_symbol_global(const char* symbol_name);
void free_all_scopes();
void print_symbols_table();

#endif // SYMBOLS_TABLE_H