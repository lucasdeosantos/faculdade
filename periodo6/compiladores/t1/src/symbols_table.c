#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols_table.h"

static Scope *currentScope = NULL;

void enter_scope() {
    Scope *newScope = malloc(sizeof(Scope));
    newScope->symbols = NULL;
    newScope->next = currentScope;
    currentScope = newScope;
}

void exit_scope() {
    if (!currentScope) {
        fprintf(stderr, "Error: No scope to exit.\n");
        exit(1);
    }

    Symbol *s = currentScope->symbols;
    while (s) {
        Symbol *next = s->next;
        free(s->name);
        free(s);
        s = next;
    }

    Scope *oldScope = currentScope;
    currentScope = currentScope->next;
    free(oldScope);
}

Symbol *lookup_symbol_current_scope(const char *name) {
    if (!currentScope) return NULL;

    for (Symbol *s = currentScope->symbols; s != NULL; s = s->next) {
        if (strcmp(s->name, name) == 0)
            return s;
    }
    return NULL;
}

void insert_symbol(const char *name, VarType type) {
    if (!currentScope) {
        fprintf(stderr, "Error: No scope to insert symbol.\n");
        exit(1);
    }

    if (lookup_symbol_current_scope(name)) {
        fprintf(stderr, "Error: Symbol '%s' already declared in this scope.\n", name);
        exit(1);
    }

    Symbol *sym = malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = type;
    sym->next = currentScope->symbols;
    currentScope->symbols = sym;
}

Symbol *lookup_symbol(const char *name) {
    for (Scope *scope = currentScope; scope != NULL; scope = scope->next) {
        for (Symbol *s = scope->symbols; s != NULL; s = s->next) {
            if (strcmp(s->name, name) == 0)
                return s;
        }
    }
    return NULL;
}

void free_all_scopes() {
    while (currentScope != NULL) {
        exit_scope();
    }
}
