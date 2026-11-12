#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symbols_table.h"

Scope *current_scope = NULL;
Scope *global_scope_head = NULL;
int semantic_errors_count = 0;

void init_symbol_table() {
    current_scope = NULL;
    global_scope_head = NULL;
    semantic_errors_count = 0;
}

void enter_scope(char *scope_name) {
    Scope *new_scope = (Scope *)malloc(sizeof(Scope));
    if (new_scope == NULL) {
        fprintf(stderr, "[ERROR] Erro ao alocar memória para o novo escopo.\n");
        exit(EXIT_FAILURE);
    }

    new_scope->name = strdup(scope_name);
    if (new_scope->name == NULL) {
        fprintf(stderr, "[ERROR] Erro ao duplicar o nome do escopo '%s'.\n", scope_name);
        free(new_scope);
        exit(EXIT_FAILURE);
    }

    new_scope->head = NULL;
    new_scope->parent = current_scope;
    new_scope->first_child = NULL;
    new_scope->next_sibling = NULL;

    if (current_scope == NULL) {
        new_scope->level = 0;
        global_scope_head = new_scope;
    } 
    else {
        new_scope->level = current_scope->level + 1;

        Scope *child_anchor = current_scope->first_child;
        if (child_anchor == NULL) {
            current_scope->first_child = new_scope;
        } 
        else {
            while (child_anchor->next_sibling != NULL) 
                child_anchor = child_anchor->next_sibling;
    
            child_anchor->next_sibling = new_scope;
        }
    }
    current_scope = new_scope;
}

void exit_scope() {
    if (current_scope == NULL) {
        fprintf(stderr, "[ERROR] Tentativa de sair de um escopo inexistente.\n");
        return;
    }

    current_scope = current_scope->parent;
}

void insert_symbol(char *name, SymbolCategory category, ASTDataType data_type, int is_reference, ASTNode *ast_node_def) {
    if (current_scope == NULL) {
        fprintf(stderr, "[ERROR] Nenhum escopo ativo para inserir o símbolo '%s'. Chame enter_scope() primeiro.\n", name);
        exit(EXIT_FAILURE);
    }

    if (lookup_symbol_in_current_scope(name) != NULL) {
        fprintf(stderr, "[SEMANTIC ERROR] Símbolo '%s' já declarado no escopo atual '%s'.\n", name, current_scope->name);
        semantic_errors_count++;
        return;
    }

    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    if (new_symbol == NULL) {
        fprintf(stderr, "[ERROR] Erro ao alocar memória para o novo símbolo '%s'.\n", name);
        exit(EXIT_FAILURE);
    }

    new_symbol->name = strdup(name);
    if (new_symbol->name == NULL) {
        fprintf(stderr, "[ERROR] Erro ao duplicar o nome do símbolo '%s'.\n", name);
        free(new_symbol);
        exit(EXIT_FAILURE);
    }

    new_symbol->category = category;
    new_symbol->data_type = data_type;
    new_symbol->is_reference = is_reference;
    new_symbol->ast_node_def = ast_node_def; 

    new_symbol->next = current_scope->head;
    current_scope->head = new_symbol;
}

Symbol *lookup_symbol_in_current_scope(const char *name) {
    if (current_scope == NULL) return NULL;

    Symbol *current = current_scope->head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) return current;
        current = current->next;
    }
    return NULL;
}

Symbol *lookup_symbol(const char *name) {
    Scope *scope_ptr = current_scope;
    while (scope_ptr != NULL) {
        Symbol *symbol = scope_ptr->head;
        while (symbol != NULL) {
            if (strcmp(symbol->name, name) == 0) return symbol;
            symbol = symbol->next;
        }
        scope_ptr = scope_ptr->parent;
    }
    return NULL;
}

Symbol* lookup_symbol_global(const char* symbol_name) {
    if (!global_scope_head || !symbol_name) return NULL;

    Symbol* current_symbol = global_scope_head->head;
    while (current_symbol) {
        if (strcmp(current_symbol->name, symbol_name) == 0) {
            return current_symbol;
        }
        current_symbol = current_symbol->next;
    }
    return NULL;
}

Scope* find_scope_by_name_recursive(Scope* current, const char* scope_name) {
    if (!current) return NULL;

    if (current->name && strcmp(current->name, scope_name) == 0) return current;

    Scope* found_in_child = find_scope_by_name_recursive(current->first_child, scope_name);
    if (found_in_child) return found_in_child;

    Scope* found_in_sibling = find_scope_by_name_recursive(current->next_sibling, scope_name);
    if (found_in_sibling) return found_in_sibling;
    
    return NULL;
}

Symbol* lookup_symbol_in_scope(const char* scope_name, const char* symbol_name) {
    if (!scope_name || !symbol_name) return NULL;
    
    Scope* target_scope = find_scope_by_name_recursive(global_scope_head, scope_name);
    if (!target_scope) return NULL;

    Symbol* current_symbol = target_scope->head;
    while (current_symbol) {
        if (strcmp(current_symbol->name, symbol_name) == 0) return current_symbol;
        current_symbol = current_symbol->next;
    }

    return NULL;
}

static void free_symbols_in_scope(Symbol *head) {
    Symbol *current_symbol = head;
    while (current_symbol != NULL) {
        Symbol *temp = current_symbol;
        current_symbol = current_symbol->next;
        free(temp->name);
        free(temp);
    }
}

void free_all_scopes_recursive(Scope *scope) {
    if (scope == NULL) return;

    free_symbols_in_scope(scope->head);

    Scope *child = scope->first_child;
    while (child != NULL) {
        Scope *next_child = child->next_sibling;
        free_all_scopes_recursive(child);
        child = next_child;
    }

    free(scope->name);
    free(scope);
}

void free_all_scopes() {
    free_all_scopes_recursive(global_scope_head);
    global_scope_head = NULL;
    current_scope = NULL;
}

static const char* symbol_category_to_string(SymbolCategory category) {
    switch (category) {
        case SYMBOL_TYPE_VAR: return "VAR";
        case SYMBOL_TYPE_FUNCTION: return "FUNCTION";
        case SYMBOL_TYPE_PROCEDURE: return "PROCEDURE";
        case SYMBOL_TYPE_PROGRAM: return "PROGRAM";
        default: return "UNKNOWN";
    }
}

static const char* data_type_to_string_for_symbols(ASTDataType type) {
    switch (type) {
        case TYPE_INTEGER: return "Integer";
        case TYPE_REAL: return "Real";
        case TYPE_NONE: return "None";
        default: return "UnknownType";
    }
}

static void print_scope_recursive(Scope *scope) {
    if (scope == NULL) return;

    for (int i = 0; i < scope->level; ++i) fprintf(stdout, "  ");
    fprintf(stdout, "Scope Level %d: '%s' (Address: %p, Parent: %p):\n", scope->level, scope->name ? scope->name : "Unnamed", (void*)scope, (void*)scope->parent);

    if (scope->head == NULL) {
        for (int i = 0; i < scope->level; ++i) fprintf(stdout, "  ");
        fprintf(stdout, "  (Empty)\n");
    } 
    else {
        Symbol *current_symbol = scope->head;
        while (current_symbol) {
            for (int i = 0; i < scope->level; ++i) fprintf(stdout, "  ");
            fprintf(stdout, "  - Name: %s, Category: %s, Type: %s, Ref: %d, AST Def: %p\n", 
                    current_symbol->name,
                    symbol_category_to_string(current_symbol->category),
                    data_type_to_string_for_symbols(current_symbol->data_type),
                    current_symbol->is_reference,
                    (void*)current_symbol->ast_node_def);
            current_symbol = current_symbol->next;
        }
    }

    Scope *child = scope->first_child;
    while (child != NULL) {
        print_scope_recursive(child);
        child = child->next_sibling;
    }
}

void print_symbols_table() {
    fprintf(stdout, "\n--- Full Symbol Table Structure ---\n");
    if (global_scope_head == NULL)
        fprintf(stdout, "No scopes found.\n");
    else
        print_scope_recursive(global_scope_head);
        
    fprintf(stdout, "-----------------------------------\n\n");
}