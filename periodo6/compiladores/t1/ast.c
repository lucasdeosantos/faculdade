#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Erro de alocação de memória!\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

static char *safe_strdup(const char *s) {
    char *new_s = strdup(s);
    if (new_s == NULL) {
        fprintf(stderr, "Erro de alocação de memória para string!\n");
        exit(EXIT_FAILURE);
    }
    return new_s;
}

ASTNode *create_node(ASTNodeType type, ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *)safe_malloc(sizeof(ASTNode));
    node->type = type;
    node->left = left;
    node->right = right;
    node->id = NULL;
    node->value = 0;
    node->op = 0;
    node->is_reference = 0;
    node->data_type = TYPE_NONE;
    node->next = NULL;
    return node;
}

ASTNode *create_num(double value) {
    ASTNode *node = create_node(AST_NUM, NULL, NULL);
    node->value = value;
    node->data_type = TYPE_REAL;
    return node;
}

ASTNode *create_var(char *id) {
    ASTNode *node = create_node(AST_VAR, NULL, NULL);
    node->id = safe_strdup(id);
    return node;
}

ASTNode *create_read(char *id) {
    ASTNode *node = create_node(AST_READ, NULL, NULL);
    node->id = safe_strdup(id);
    return node;
}

ASTNode *create_write(ASTNode *expr) {
    return create_node(AST_WRITE, expr, NULL);
}

ASTNode *create_binop(int op, ASTNode *left, ASTNode *right) {
    ASTNode *node = create_node(AST_BINOP, left, right);
    node->op = op;
    return node;
}

ASTNode *create_unop(int op, ASTNode *operand) {
    ASTNode *node = create_node(AST_UNOP, operand, NULL);
    node->op = op;
    return node;
}

ASTNode *create_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode *node = create_node(AST_IF, cond, then_branch);
    node->right = then_branch;
    node->next = else_branch;
    return node;
}

ASTNode *create_while(ASTNode *cond, ASTNode *body) {
    ASTNode *node = create_node(AST_WHILE, cond, body);
    return node;
}

ASTNode *create_call(char *id, ASTNode *args) {
    ASTNode *node = create_node(AST_CALL, args, NULL);
    node->id = safe_strdup(id);
    return node;
}

ASTNode *create_param(char *id, ASTDataType type, int is_ref) {
    ASTNode *node = create_node(AST_PARAM, NULL, NULL);
    node->id = safe_strdup(id);
    node->data_type = type;
    node->is_reference = is_ref;
    return node;
}

ASTNode *create_arg_list(ASTNode *arg) {
    ASTNode *node = create_node(AST_ARG_LIST, arg, NULL);
    return node;
}

void print_indent(int indent_level) {
    for (int i = 0; i < indent_level; ++i) {
        printf("  ");
    }
}

void print_ast_internal(ASTNode *node, int indent_level) {
    if (!node) return;

    print_indent(indent_level);

    switch (node->type) {
        case AST_PROG:
            printf("PROGRAM:\n");
            print_indent(indent_level + 1); printf("Declarations:\n");
            print_ast_internal(node->left, indent_level + 2);

            print_indent(indent_level + 1); printf("Subprograms:\n");
            ASTNode *current_subprogram = node->right;
            while (current_subprogram) {
                print_ast_internal(current_subprogram, indent_level + 2);
                current_subprogram = current_subprogram->next;
            }

            print_indent(indent_level + 1); printf("Main Body:\n");
            ASTNode *current_main_stmt = node->next;
            while (current_main_stmt) {
                print_ast_internal(current_main_stmt, indent_level + 2);
                current_main_stmt = current_main_stmt->next;
            }
            break;

        case AST_DECLARATIONS:
            printf("DECLARATIONS:\n");
            ASTNode *current_declared_var = node->left;
            while (current_declared_var) {
                print_ast_internal(current_declared_var, indent_level + 1);
                current_declared_var = current_declared_var->next;
            }
            if (node->right) {
                print_ast_internal(node->right, indent_level + 1);
            }
            break;

        case AST_FUNCTION:
            printf("FUNCTION: %s (Type: %s)\n", node->left->id, node->data_type == TYPE_INTEGER ? "Integer" : "Real");
            print_indent(indent_level + 1); printf("Parameters:\n");
            ASTNode *func_param = node->left->right;
            while (func_param) {
                print_ast_internal(func_param, indent_level + 2);
                func_param = func_param->next;
            }
            print_indent(indent_level + 1); printf("Local Declarations:\n");
            print_ast_internal(node->left->left, indent_level + 2);

            print_indent(indent_level + 1); printf("Body:\n");
            ASTNode *func_body_stmt = node->right;
            while (func_body_stmt) {
                print_ast_internal(func_body_stmt, indent_level + 2);
                func_body_stmt = func_body_stmt->next;
            }
            break;

        case AST_PROCEDURE:
            printf("PROCEDURE: %s\n", node->left->id);
            print_indent(indent_level + 1); printf("Parameters:\n");
            ASTNode *proc_param = node->left->right;
            while (proc_param) {
                print_ast_internal(proc_param, indent_level + 2);
                proc_param = proc_param->next;
            }
            print_indent(indent_level + 1); printf("Local Declarations:\n");
            print_ast_internal(node->left->left, indent_level + 2);

            print_indent(indent_level + 1); printf("Body:\n");
            ASTNode *proc_body_stmt = node->right;
            while (proc_body_stmt) {
                print_ast_internal(proc_body_stmt, indent_level + 2);
                proc_body_stmt = proc_body_stmt->next;
            }
            break;

        case AST_NUM:
            printf("NUM(%.2f, Type: %s)\n", node->value, node->data_type == TYPE_INTEGER ? "Integer" : "Real");
            break;

        case AST_VAR:
            printf("VAR(%s, Type: %s)\n", node->id, node->data_type == TYPE_INTEGER ? "Integer" : (node->data_type == TYPE_REAL ? "Real" : "Unknown"));
            break;

        case AST_BINOP:
            printf("BINOP('%c', Type: %s)\n", node->op, node->data_type == TYPE_INTEGER ? "Integer" : (node->data_type == TYPE_REAL ? "Real" : "Unknown"));
            print_ast_internal(node->left, indent_level + 1);
            print_ast_internal(node->right, indent_level + 1);
            break;

        case AST_UNOP:
            printf("UNOP('%c', Type: %s)\n", node->op, node->data_type == TYPE_INTEGER ? "Integer" : (node->data_type == TYPE_REAL ? "Real" : "Unknown"));
            print_ast_internal(node->left, indent_level + 1);
            break;

        case AST_READ:
            printf("READ:\n");
            ASTNode *read_var = node->left;
            while(read_var) {
                print_ast_internal(read_var, indent_level + 1);
                read_var = read_var->next;
            }
            break;

        case AST_WRITE:
            printf("WRITE:\n");
            print_ast_internal(node->left, indent_level + 1);
            break;

        case AST_WRITELN:
            printf("WRITELN:\n");
            break;

        case AST_IF:
            printf("IF:\n");
            print_indent(indent_level + 1); printf("Condition:\n");
            print_ast_internal(node->left, indent_level + 2);

            print_indent(indent_level + 1); printf("THEN:\n");
            ASTNode *then_branch_stmt = node->right;
            while(then_branch_stmt) {
                print_ast_internal(then_branch_stmt, indent_level + 2);
                then_branch_stmt = then_branch_stmt->next;
            }

            if (node->next) {
                print_indent(indent_level + 1); printf("ELSE:\n");
                ASTNode *else_branch_stmt = node->next;
                while(else_branch_stmt) {
                    print_ast_internal(else_branch_stmt, indent_level + 2);
                    else_branch_stmt = else_branch_stmt->next;
                }
            }
            break;

        case AST_WHILE:
            printf("WHILE:\n");
            print_indent(indent_level + 1); printf("Condition:\n");
            print_ast_internal(node->left, indent_level + 2);

            print_indent(indent_level + 1); printf("Body:\n");
            ASTNode *while_body_stmt = node->right;
            while (while_body_stmt) {
                print_ast_internal(while_body_stmt, indent_level + 2);
                while_body_stmt = while_body_stmt->next;
            }
            break;

        case AST_CALL:
            printf("CALL(%s)\n", node->id);
            if (node->left) {
                print_indent(indent_level + 1); printf("Arguments:\n");
                ASTNode *arg = node->left;
                while (arg) {
                    print_ast_internal(arg, indent_level + 2);
                    arg = arg->next;
                }
            }
            break;

        case AST_PARAM:
            printf("PARAM(%s, Type: %s, Ref: %d)\n", node->id,
                   node->data_type == TYPE_INTEGER ? "Integer" : "Real",
                   node->is_reference);
            break;

        case AST_ARG_LIST:
            printf("ARGUMENT LIST (Container):\n");
            ASTNode *current_arg_expr = node->left;
            while (current_arg_expr) {
                print_ast_internal(current_arg_expr, indent_level + 1);
                current_arg_expr = current_arg_expr->next;
            }
            break;

        case AST_STATEMENT:
            printf("STATEMENT (Generic):\n");
            print_ast_internal(node->left, indent_level + 1);
            break;

        default:
            printf("UNKNOWN AST TYPE (%d)\n", node->type);
            break;
    }
}

void print_ast(ASTNode *node) {
    print_ast_internal(node, 0);
}

void free_ast(ASTNode *node) {
    if (!node) return;

    free_ast(node->left);

    if (node->right && node->right != node->next)
        free_ast(node->right);

    if (node->next && node->next != node->left && node->next != node->right)
        free_ast(node->next);

    if (node->id)
        free(node->id);

    free(node);
}