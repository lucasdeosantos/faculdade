#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "symbols_table.h"

char *safe_strdup(const char *s) {
    if (s == NULL) return NULL;
    char *new_s = strdup(s);
    if (new_s == NULL) {
        fprintf(stderr, "[ERROR] Erro de alocação de memória para strdup.\n");
        exit(EXIT_FAILURE);
    }
    return new_s;
}

ASTNode *create_node(ASTNodeType type, ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "[ERROR] Erro de alocação de memória para ASTNode.\n");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->left = left;
    node->right = right;
    node->id = NULL;
    node->value = 0.0;
    node->op = 0;
    node->is_reference = 0;
    node->data_type = TYPE_NONE;
    node->next = NULL;
    return node;
}

ASTNode *create_num(int value) {
    ASTNode *node = create_node(AST_NUM, NULL, NULL);
    node->value = value;
    node->data_type = TYPE_INTEGER;
    return node;
}

ASTNode *create_var(char *id) {
    ASTNode *node = create_node(AST_VAR, NULL, NULL);
    node->id = safe_strdup(id);
    return node;
}

ASTNode *create_read(ASTNode *id_list) {
    ASTNode *node = create_node(AST_READ, NULL, NULL);
    node->left = id_list;
    return node;
}

ASTNode *create_write(ASTNode *expr) {
    ASTNode *node = create_node(AST_WRITE, expr, NULL);
    return node;
}

ASTNode *create_binop(ASTOperator op, ASTNode *left, ASTNode *right) {
    ASTNode *node = create_node(AST_BINOP, left, right);
    node->op = op;
    return node;
}

ASTNode *create_unop(ASTOperator op, ASTNode *operand) {
    ASTNode *node = create_node(AST_UNOP, operand, NULL);
    node->op = op;
    return node;
}

ASTNode *create_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode *node = create_node(AST_IF, cond, create_node(AST_IF_BLOCK, then_branch, else_branch));
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
    return arg;
}

ASTNode *create_assignment(ASTNode *var, ASTNode *expr) {
    ASTNode *node = create_node(AST_ASSIGNMENT, var, expr);
    return node;
}

ASTNode *create_function_block(ASTNode *declarations, ASTNode *body) {
    ASTNode *node = create_node(AST_FUNCTION_BLOCK, declarations, body);
    return node;
}

ASTNode *create_function(char *id, ASTNode *params, ASTDataType return_type, ASTNode *block) {
    ASTNode *node = create_node(AST_FUNCTION, params, block);
    node->id = safe_strdup(id);
    node->data_type = return_type;
    return node;
}

ASTNode *create_procedure(char *id, ASTNode *params, ASTNode *block) {
    ASTNode *node = create_node(AST_PROCEDURE, params, block);
    node->id = safe_strdup(id);
    node->data_type = TYPE_NONE;
    return node;
}

ASTNode *create_program(char *id, ASTNode *global_decls, ASTNode *subprograms, ASTNode *main_body) {
    ASTNode *node = create_node(AST_PROG, global_decls, subprograms);
    node->next = main_body;
    node->id = id;
    return node;
}

void print_indent(int indent) {
    for (int i = 0; i < indent; i++)
        printf(" ");
}

const char* data_type_to_string(ASTDataType type) {
    switch (type) {
        case TYPE_INTEGER: return "Integer";
        case TYPE_REAL: return "Real";
        case TYPE_NONE: return "None";
        default: return "UnknownType";
    }
}

const char* operator_to_string(ASTOperator op) {
    switch (op) {
        case OP_EQ: return "EQ";
        case OP_NE: return "NE";
        case OP_LT: return "LT";
        case OP_LE: return "LE";
        case OP_GT: return "GT";
        case OP_GE: return "GE";
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_OR: return "OR";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_DIV_INT: return "DIV";
        case OP_MOD: return "MOD";
        case OP_AND: return "AND";
        case OP_UMINUS: return "U_MINUS";
        case OP_UPLUS: return "U_PLUS";
        default: return "UnknownOp";
    }
}

void print_ast(ASTNode *node, int indent_level) {
    if (!node) return; 

    print_indent(indent_level);

    switch (node->type) {
        case AST_PROG:
            printf("PROGRAM: (Name: %s)\n", node->id ? node->id : "Unnamed");
            if (node->left) {
                print_indent(indent_level + 2); printf("Declarations:\n");
                print_ast(node->left, indent_level + 4);
            }
            if (node->right) {
                print_indent(indent_level + 2); printf("Subprograms:\n");
                print_ast(node->right, indent_level + 4);
            }
            if (node->next) {
                print_indent(indent_level + 2); printf("Main Body:\n");
                print_ast(node->next, indent_level + 4);
            }
            break;

        case AST_DECLARATIONS:
            printf("DECLARATIONS:\n");
            print_ast(node->left, indent_level + 2);
            if (node->right) {
                 print_ast(node->right, indent_level);
            }
            break;

        case AST_VAR:
            printf("VAR(%s, Type: %s)\n", node->id, data_type_to_string(node->data_type));
            print_ast(node->next, indent_level);
            break;

        case AST_FUNCTION:
        case AST_PROCEDURE:
            printf("%s: %s (Type: %s)\n",
                   (node->type == AST_FUNCTION ? "FUNCTION" : "PROCEDURE"),
                   node->id, data_type_to_string(node->data_type));
            if (node->left) {
                print_indent(indent_level + 2); printf("Parameters:\n");
                print_ast(node->left, indent_level + 4); 
            }
            if (node->right) {
                print_indent(indent_level + 2); printf("%s Block:\n", (node->type == AST_FUNCTION ? "Function" : "Procedure"));
                print_ast(node->right, indent_level + 4);
            }
            print_ast(node->next, indent_level);
            break;

        case AST_PARAM:
            printf("PARAM(%s, Type: %s, Ref: %d)\n", node->id, data_type_to_string(node->data_type), node->is_reference);
            print_ast(node->next, indent_level);
            break;

        case AST_FUNCTION_BLOCK:
            printf("FUNCTION BLOCK:\n");
            if (node->left) {
                print_indent(indent_level + 2); printf("Local Declarations:\n");
                print_ast(node->left, indent_level + 4);
            }
            if (node->right) {
                print_indent(indent_level + 2); printf("Body:\n");
                print_ast(node->right, indent_level + 4);
            }
            break;

        case AST_ASSIGNMENT:
            printf("ASSIGNMENT:\n");
            print_indent(indent_level + 2); printf("Variable:\n");
            print_ast(node->left, indent_level + 4);
            print_indent(indent_level + 2); printf("Expression:\n");
            print_ast(node->right, indent_level + 4);
            print_ast(node->next, indent_level);
            break;

        case AST_BINOP:
            printf("BINOP(%s, Type: %s)\n", operator_to_string(node->op), data_type_to_string(node->data_type));
            print_ast(node->left, indent_level + 2);
            print_ast(node->right, indent_level + 2);
            break;

        case AST_UNOP:
            printf("UNOP(%s, Type: %s)\n", operator_to_string(node->op), data_type_to_string(node->data_type));
            print_ast(node->left, indent_level + 2);
            break;

        case AST_NUM:
            printf("NUM(%.2f, Type: %s)\n", node->value, data_type_to_string(node->data_type));
            break;

        case AST_CALL:
            printf("CALL(%s, Type: %s)\n", node->id, data_type_to_string(node->data_type));
            if (node->left) {
                print_indent(indent_level + 2); printf("Arguments:\n");
                print_ast(node->left, indent_level + 4);
            }
            print_ast(node->next, indent_level);
            break;

        case AST_READ:
            printf("READ:\n");
            if (node->left) {
                print_indent(indent_level + 2); printf("Into Variables:\n");
                print_ast(node->left, indent_level + 4);
            }
            print_ast(node->next, indent_level);
            break;

        case AST_WRITE:
            printf("WRITE:\n");
            if (node->left) {
                print_indent(indent_level + 2); printf("Expression:\n");
                print_ast(node->left, indent_level + 4);
            }
            print_ast(node->next, indent_level);
            break;

        case AST_WRITELN:
            printf("WRITELN\n");
            print_ast(node->next, indent_level);
            break;

        case AST_IF:
            printf("IF:\n");
            print_indent(indent_level + 2); printf("Condition:\n");
            print_ast(node->left, indent_level + 4);
            print_ast(node->right, indent_level + 2);
            print_ast(node->next, indent_level);
            break;

        case AST_IF_BLOCK:
            printf("IF_BLOCK:\n");
            print_indent(indent_level + 2); printf("Then Branch:\n");
            ASTNode *current_then_stmt = node->left;
            while (current_then_stmt) {
                print_ast(current_then_stmt, indent_level + 4);
                current_then_stmt = current_then_stmt->next;
            }
            print_indent(indent_level + 2); printf("Else Branch:\n");
            ASTNode *current_else_stmt = node->right;
            while (current_else_stmt) {
                print_ast(current_else_stmt, indent_level + 4);
                current_else_stmt = current_else_stmt->next;
            }
            break;

        case AST_WHILE:
            printf("WHILE:\n");
            print_indent(indent_level + 2); printf("Condition:\n");
            print_ast(node->left, indent_level + 4);
            print_indent(indent_level + 2); printf("Body:\n");
            print_ast(node->right, indent_level + 4);
            print_ast(node->next, indent_level);
            break;

        default:
            printf("UNKNOWN_NODE_TYPE (%d)\n", node->type);
            print_ast(node->left, indent_level + 2);
            print_ast(node->right, indent_level + 2);
            print_ast(node->next, indent_level);
            break;
    }
}

void free_ast(ASTNode *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->next);
    if (node->id) free(node->id);
    free(node);
}

ASTDataType perform_semantic_analysis(ASTNode *node) {
    if (node == NULL) {
        return TYPE_NONE;
    }

    ASTDataType left_type, right_type;
    ASTDataType current_node_type = TYPE_NONE;

    switch (node->type) {
        case AST_PROG:
            if (node->left)
                perform_semantic_analysis(node->left);
            if (node->right)
                perform_semantic_analysis(node->right);
            if (node->next)
                perform_semantic_analysis(node->next);

            current_node_type = TYPE_NONE;
            break;

        case AST_DECLARATIONS:
            if (node->left)
                perform_semantic_analysis(node->left);
            if (node->right)
                perform_semantic_analysis(node->right);

            current_node_type = TYPE_NONE;
            break;

        case AST_FUNCTION:
        case AST_PROCEDURE:
            if (node->left) {
                ASTNode *param_node = node->left;
                while (param_node != NULL) {
                    perform_semantic_analysis(param_node);
                    param_node = param_node->next;
                }
            }
            if (node->right)
                perform_semantic_analysis(node->right);
            if (node->next)
                perform_semantic_analysis(node->next);

            current_node_type = node->data_type;
            break;
        
        case AST_FUNCTION_BLOCK:
            if (node->left)
                perform_semantic_analysis(node->left);
    
            if (node->right) {
                ASTNode *current_stmt = node->right;
                while (current_stmt != NULL) {
                    perform_semantic_analysis(current_stmt);
                    current_stmt = current_stmt->next;
                }
            }
            current_node_type = TYPE_NONE;
            break;

        case AST_VAR:
            current_node_type = node->data_type;
            if (node->next)
                perform_semantic_analysis(node->next);
            break;

        case AST_PARAM:
            current_node_type = node->data_type;
            if (node->next)
                perform_semantic_analysis(node->next);
            break;

        case AST_NUM:
            current_node_type = node->data_type;
            break;

        case AST_BINOP:
            left_type = perform_semantic_analysis(node->left);
            right_type = perform_semantic_analysis(node->right);

            if (left_type == TYPE_NONE || right_type == TYPE_NONE) {
                node->data_type = TYPE_NONE;
            } 
            else {
                switch (node->op) {
                    case OP_PLUS:
                    case OP_MINUS:
                    case OP_MUL:
                        if (left_type == TYPE_INTEGER && right_type == TYPE_INTEGER) {
                            node->data_type = TYPE_INTEGER;
                        } 
                        else if (left_type == TYPE_REAL || right_type == TYPE_REAL) {
                            node->data_type = TYPE_REAL;
                        } 
                        else {
                            fprintf(stderr, "[SEMANTIC ERROR] Tipos incompatíveis para operação aritmética '%s': %s e %s.\n",
                                    operator_to_string(node->op), data_type_to_string(left_type), data_type_to_string(right_type));
                            semantic_errors_count++;
                            node->data_type = TYPE_NONE;
                        }
                        break;
                    case OP_DIV_INT:
                    case OP_MOD:
                        if (left_type == TYPE_INTEGER && right_type == TYPE_INTEGER) {
                            node->data_type = TYPE_INTEGER;
                        } 
                        else {
                            fprintf(stderr, "[SEMANTIC ERROR] Operador '%s' requer operandos inteiros, mas encontrou %s e %s.\n",
                                    operator_to_string(node->op), data_type_to_string(left_type), data_type_to_string(right_type));
                            semantic_errors_count++;
                            node->data_type = TYPE_NONE;
                        }
                        break;
                    case OP_DIV:
                        if ((left_type == TYPE_INTEGER || left_type == TYPE_REAL) &&
                            (right_type == TYPE_INTEGER || right_type == TYPE_REAL)) {
                            node->data_type = TYPE_REAL;
                        } 
                        else {
                            fprintf(stderr, "[SEMANTIC ERROR] Tipos incompatíveis para divisão real '%s': %s e %s.\n",
                                    operator_to_string(node->op), data_type_to_string(left_type), data_type_to_string(right_type));
                            semantic_errors_count++;
                            node->data_type = TYPE_NONE;
                        }
                        break;
                    case OP_EQ: case OP_NE: case OP_LT: case OP_LE: case OP_GT: case OP_GE:
                        if ((left_type == TYPE_INTEGER && right_type == TYPE_INTEGER) ||
                            (left_type == TYPE_REAL && right_type == TYPE_REAL) ||
                            (left_type == TYPE_INTEGER && right_type == TYPE_REAL) ||
                            (left_type == TYPE_REAL && right_type == TYPE_INTEGER)
                        ) {
                            node->data_type = TYPE_INTEGER;
                        } 
                        else {
                            fprintf(stderr, "[SEMANTIC ERROR] Tipos incompatíveis para comparação '%s': %s e %s.\n",
                                    operator_to_string(node->op), data_type_to_string(left_type), data_type_to_string(right_type));
                            semantic_errors_count++;
                            node->data_type = TYPE_NONE;
                        }
                        break;
                    case OP_AND: case OP_OR:
                        if (left_type == TYPE_INTEGER && right_type == TYPE_INTEGER) {
                            node->data_type = TYPE_INTEGER;
                        } 
                        else {
                            fprintf(stderr, "[SEMANTIC ERROR] Operadores lógicos '%s' exigem operandos inteiros (0/1), mas encontrou %s e %s.\n",
                                    operator_to_string(node->op), data_type_to_string(left_type), data_type_to_string(right_type));
                            semantic_errors_count++;
                            node->data_type = TYPE_NONE;
                        }
                        break;
                    default:
                        fprintf(stderr, "[COMPILATION ERROR] Operador binário desconhecido na análise semântica: %s.\n", operator_to_string(node->op));
                        semantic_errors_count++;
                        node->data_type = TYPE_NONE;
                        break;
                }
            }
            current_node_type = node->data_type;
            break;

        case AST_UNOP:
            ASTDataType operand_type = perform_semantic_analysis(node->left);

            if (operand_type == TYPE_NONE) {
                node->data_type = TYPE_NONE;
            } 
            else {
                switch (node->op) {
                    case OP_UMINUS:
                    case OP_UPLUS:
                        if (operand_type == TYPE_INTEGER || operand_type == TYPE_REAL) {
                            node->data_type = operand_type;
                        } 
                        else {
                            fprintf(stderr, "[SEMANTIC ERROR] Operador unário '%s' requer operando numérico, mas encontrou %s.\n",
                                    operator_to_string(node->op), data_type_to_string(operand_type));
                            semantic_errors_count++;
                            node->data_type = TYPE_NONE;
                        }
                        break;
                    default:
                        fprintf(stderr, "[COMPILATION ERROR] Operador unário desconhecido na análise semântica: %s.\n", operator_to_string(node->op));
                        semantic_errors_count++;
                        node->data_type = TYPE_NONE;
                        break;
                }
            }
            current_node_type = node->data_type;
            break;

        case AST_CALL: {
            if (node->left) {
                ASTNode *current_arg_node = node->left;
                while (current_arg_node != NULL) {
                    perform_semantic_analysis(current_arg_node);
                    current_arg_node = current_arg_node->next;
                }
            }
            current_node_type = node->data_type;

            if (node->next)
                perform_semantic_analysis(node->next);
            break;
        }

        case AST_READ:
            if (node->left == NULL || node->left->type != AST_VAR) { 
                fprintf(stderr, "[SEMANTIC ERROR] 'READ' espera uma variável como argumento.\n");
                semantic_errors_count++;
            } 
            else {
                perform_semantic_analysis(node->left); 
            }
            if (node->next) {
                perform_semantic_analysis(node->next);
            }
            current_node_type = TYPE_NONE;
            break;

        case AST_WRITE:
        case AST_WRITELN:
            if (node->left)
                perform_semantic_analysis(node->left);
            if (node->next)
                perform_semantic_analysis(node->next);
            current_node_type = TYPE_NONE;
            break;

        case AST_IF:
            ASTDataType cond_type = perform_semantic_analysis(node->left);
            if (cond_type != TYPE_INTEGER) {
                fprintf(stderr, "[SEMANTIC ERROR] Condição do IF deve ser numérica (booleana), mas encontrou %s.\n",
                                data_type_to_string(cond_type));
                semantic_errors_count++;
            }
            perform_semantic_analysis(node->right);

            if (node->next)
                perform_semantic_analysis(node->next);
            current_node_type = TYPE_NONE;
            break;

        case AST_IF_BLOCK:
            ASTNode *current_then_stmt = node->left;
            while (current_then_stmt) {
                perform_semantic_analysis(current_then_stmt);
                current_then_stmt = current_then_stmt->next;
            }
            ASTNode *current_else_stmt = node->right;
            while (current_else_stmt) {
                perform_semantic_analysis(current_else_stmt);
                current_else_stmt = current_else_stmt->next;
            }
            current_node_type = TYPE_NONE;
            break;

        case AST_WHILE:
            cond_type = perform_semantic_analysis(node->left);
            if (cond_type != TYPE_INTEGER) {
                fprintf(stderr, "[SEMANTIC ERROR] Condição do WHILE deve ser numérica (booleana), mas encontrou %s.\n",
                                data_type_to_string(cond_type));
                semantic_errors_count++;
            }
            perform_semantic_analysis(node->right);
            if (node->next)
                perform_semantic_analysis(node->next);
            current_node_type = TYPE_NONE;
            break;

        case AST_ASSIGNMENT:
            left_type = perform_semantic_analysis(node->left);
            right_type = perform_semantic_analysis(node->right);

            if (left_type == TYPE_NONE || right_type == TYPE_NONE) {
                node->data_type = TYPE_NONE;
            }
            else if (left_type == right_type) {
                node->data_type = left_type;
            } 
            else if (left_type == TYPE_REAL && right_type == TYPE_INTEGER) {
                node->data_type = TYPE_REAL;
            } 
            else {
                fprintf(stderr, "[SEMANTIC ERROR] Incompatibilidade de tipo na atribuição. Não é possível atribuir %s a %s.\n",
                                data_type_to_string(right_type), data_type_to_string(left_type));
                semantic_errors_count++;
                node->data_type = TYPE_NONE;
            }
            current_node_type = node->data_type;

            if (node->next)
                perform_semantic_analysis(node->next);
            break;

        case AST_ARG_LIST:
            if (node->left) {
                ASTNode *current_arg = node->left;
                while(current_arg != NULL) {
                    perform_semantic_analysis(current_arg);
                    current_arg = current_arg->next;
                }
            }
            current_node_type = TYPE_NONE;
            break;

        default:
            fprintf(stderr, "[COMPILATION ERROR] Tipo de nó AST não reconhecido na análise semântica: %d.\n", node->type);
            semantic_errors_count++;
            current_node_type = TYPE_NONE;
            break;
    }
    return current_node_type;
}