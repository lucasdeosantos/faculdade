#ifndef SYMBOLS_TABLE_H
#define SYMBOLS_TABLE_H

typedef enum { TYPE_INTEGER, TYPE_REAL, TYPE_VOID } VarType;

typedef struct Symbol {
    char *name;
    VarType type;
    struct Symbol *next;
} Symbol;

typedef struct Scope {
    Symbol *symbols;
    struct Scope *next;
} Scope;

void enter_scope();
void exit_scope();

void insert_symbol(const char *name, VarType type);
Symbol *lookup_symbol(const char *name);

void free_all_scopes();

#endif // SYMBOLS_TABLE_H