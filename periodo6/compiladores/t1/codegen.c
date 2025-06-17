#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codegen.h"
#include "symbols_table.h"
#include "ast.h"

static FILE *out;
static char *current_scope_name = NULL;
static char *global_scope_name = NULL;
static int register_counter = 0;
static int if_counter = 0;
static int while_counter = 0;

static int get_next_register() {
    return register_counter++;
}

LLVMValue create_llvm_value(int reg_id, ASTDataType type) {
    LLVMValue val;
    val.reg_id = reg_id;
    val.type = type;
    return val;
}

static char *get_llvm_type_str(ASTDataType type) {
    if (type == TYPE_INTEGER) return "i32";
    else if (type == TYPE_REAL) return "double";
    else return "void";
}

static void generate_global_declarations(ASTNode *declarations_node) {
    if (!declarations_node) return;

    ASTNode *current_var_node = declarations_node->left; 
    while (current_var_node) {
        if (current_var_node->type == AST_VAR) {
            Symbol *entry = lookup_symbol_in_scope(global_scope_name, current_var_node->id); 
            if (!entry) {
                fprintf(stderr, "[COMPILATION ERROR] Variável global '%s' não declarada ou não encontrada na tabela de símbolos.\n", current_var_node->id);
                exit(EXIT_FAILURE);
            }

            char *llvm_type_str = get_llvm_type_str(entry->data_type);
            int align = (entry->data_type == TYPE_INTEGER ? 4 : 8);

            fprintf(out, "@%s = global %s ", current_var_node->id, llvm_type_str);
            if (entry->data_type == TYPE_INTEGER)
                fprintf(out, "0, align %d\n", align);
            else
                fprintf(out, "0.0, align %d\n", align);
        } 
        else {
            fprintf(stderr, "[WARNING] Esperava-se AST_VAR para variável global, mas foi encontrado o tipo %d para o nó com ID '%s'.\n", current_var_node->type, current_var_node->id ? current_var_node->id : "NULL");
        }
        current_var_node = current_var_node->next; 
    }
    generate_global_declarations(declarations_node->right);
}   

static void generate_local_allocations(ASTNode *declarations_node) {
    if (!declarations_node) return;

    ASTNode *current_decl_node = declarations_node->left;
    while (current_decl_node) {
        if (current_decl_node->type == AST_VAR) {
            Symbol *entry = lookup_symbol_in_scope(current_scope_name, current_decl_node->id); 
            if (!entry) {
                fprintf(stderr, "[COMPILATION ERROR] Variável local '%s' não encontrada na tabela de símbolos para a função '%s'.\n", current_decl_node->id, current_scope_name ? current_scope_name : "UNKNOWN");
                exit(EXIT_FAILURE);
            }
            char *llvm_type_str = get_llvm_type_str(entry->data_type);
            int align = (entry->data_type == TYPE_INTEGER ? 4 : 8);
            fprintf(out, "\t%%%s = alloca %s, align %d\n", current_decl_node->id, llvm_type_str, align);
        } 
        else {
            fprintf(stderr, "[WARNING] Esperava-se AST_VAR para variável local, mas foi encontrado o tipo %d para o nó com ID '%s' na função '%s'.\n", current_decl_node->type, current_decl_node->id ? current_decl_node->id : "NULL", current_scope_name ? current_scope_name : "UNKNOWN");
        }
        current_decl_node = current_decl_node->next;
    }
    generate_local_allocations(declarations_node->right);
}

static LLVMValue generate_expression(ASTNode *node) {
    if (!node) {
        fprintf(stderr, "[COMPILATION ERROR] generate_expression recebeu um nó NULO.\n");
        exit(EXIT_FAILURE);
    }

    int result_reg;

    switch (node->type) {
        case AST_NUM: {
            result_reg = get_next_register();
            if (node->data_type == TYPE_INTEGER) {
                fprintf(out, "\t%%%d = add i32 0, %d\n", result_reg, (int)node->value);
            } 
            else if (node->data_type == TYPE_REAL) {
                fprintf(out, "\t%%%d = fadd double 0.0, %f\n", result_reg, node->value);
            } 
            else {
                fprintf(stderr, "[COMPILATION ERROR] Tipo de NUM desconhecido: %s.\n", data_type_to_string(node->data_type));
                exit(EXIT_FAILURE);
            }
            return create_llvm_value(result_reg, node->data_type);
        }

        case AST_VAR: {
            int global = 0;
            Symbol *entry = lookup_symbol_in_scope(current_scope_name, node->id);
            if (!entry) {
                entry = lookup_symbol_in_scope(global_scope_name, node->id);
                if (!entry) {
                    fprintf(stderr, "[COMPILATION ERROR] Variável '%s' não encontrada na tabela de símbolos na função '%s' ou escopo global.\n", node->id, current_scope_name ? current_scope_name : "UNKNOWN");
                    exit(EXIT_FAILURE);
                }
                global = 1;
            }

            char *var_llvm_type_str = get_llvm_type_str(entry->data_type);
            int var_align = (entry->data_type == TYPE_INTEGER ? 4 : 8);

            result_reg = get_next_register();
            if (global) {
                fprintf(out, "\t%%%d = load %s, ptr @%s, align %d\n", result_reg, var_llvm_type_str, node->id, var_align);
            } 
            else if (entry->is_reference) {
                int ptr_reg = get_next_register();
                fprintf(out, "\t%%%d = load ptr, ptr %%%s, align 8\n", ptr_reg, node->id);
                fprintf(out, "\t%%%d = load %s, ptr %%%d, align %d\n", result_reg, var_llvm_type_str, ptr_reg, var_align);
            }
            else {
                fprintf(out, "\t%%%d = load %s, ptr %%%s, align %d\n", result_reg, var_llvm_type_str, node->id, var_align);
            }
            return create_llvm_value(result_reg, entry->data_type);
        }

        case AST_BINOP: {
            LLVMValue left_val = generate_expression(node->left);
            LLVMValue right_val = generate_expression(node->right);
            
            ASTDataType result_ast_type = node->data_type;
            char *result_llvm_type_str = get_llvm_type_str(result_ast_type);

            if (left_val.type == TYPE_INTEGER && result_ast_type == TYPE_REAL) {
                int cast_reg = get_next_register();
                fprintf(out, "\t%%%d = sitofp i32 %%%d to double\n", cast_reg, left_val.reg_id);
                left_val.reg_id = cast_reg;
                left_val.type = TYPE_REAL;
            }
            if (right_val.type == TYPE_INTEGER && result_ast_type == TYPE_REAL) {
                int cast_reg = get_next_register();
                fprintf(out, "\t%%%d = sitofp i32 %%%d to double\n", cast_reg, right_val.reg_id);
                right_val.reg_id = cast_reg;
                right_val.type = TYPE_REAL;
            }
            
            result_reg = get_next_register();
            
            char *op_str;
            switch (node->op) {
                case OP_PLUS:    op_str = (result_ast_type == TYPE_INTEGER) ? "add" : "fadd"; break;
                case OP_MINUS:   op_str = (result_ast_type == TYPE_INTEGER) ? "sub" : "fsub"; break;
                case OP_MUL:     op_str = (result_ast_type == TYPE_INTEGER) ? "mul" : "fmul"; break;
                case OP_DIV:     op_str = "fdiv"; break;
                case OP_DIV_INT: op_str = "sdiv"; break;
                case OP_MOD:     op_str = "srem"; break;
                case OP_EQ:      op_str = (left_val.type == TYPE_INTEGER) ? "icmp eq" : "fcmp oeq"; break;
                case OP_NE:      op_str = (left_val.type == TYPE_INTEGER) ? "icmp ne" : "fcmp une"; break;
                case OP_LT:      op_str = (left_val.type == TYPE_INTEGER) ? "icmp slt" : "fcmp olt"; break;
                case OP_LE:      op_str = (left_val.type == TYPE_INTEGER) ? "icmp sle" : "fcmp ole"; break;
                case OP_GT:      op_str = (left_val.type == TYPE_INTEGER) ? "icmp sgt" : "fcmp ogt"; break;
                case OP_GE:      op_str = (left_val.type == TYPE_INTEGER) ? "icmp sge" : "fcmp oge"; break;
                case OP_AND:     op_str = "and"; break;
                case OP_OR:      op_str = "or"; break;
                default:
                    fprintf(stderr, "[COMPILATION ERROR] Operador binário não suportado: %d\n", node->op);
                    exit(EXIT_FAILURE);
            }
            
            if (node->op >= OP_EQ && node->op <= OP_GE) {
                int bool_result_reg = get_next_register();
                fprintf(out, "\t%%%d = %s %s %%%d, %%%d\n", result_reg, op_str, result_llvm_type_str, left_val.reg_id, right_val.reg_id);
                fprintf(out, "\t%%%d = zext i1 %%%d to i32\n", bool_result_reg, result_reg);
                return create_llvm_value(bool_result_reg, result_ast_type);
            } 
            else {
                fprintf(out, "\t%%%d = %s %s %%%d, %%%d\n", result_reg, op_str, result_llvm_type_str, left_val.reg_id, right_val.reg_id);
                return create_llvm_value(result_reg, result_ast_type);
            }
        }

        case AST_UNOP: {
            LLVMValue operand_val = generate_expression(node->left);
            result_reg = get_next_register();
            
            ASTDataType result_ast_type = node->data_type;
            char *result_llvm_type_str = get_llvm_type_str(result_ast_type);

            if (node->op == OP_UMINUS) {
                if (result_ast_type == TYPE_INTEGER)
                    fprintf(out, "\t%%%d = sub i32 0, %%%d\n", result_reg, operand_val.reg_id);
                else
                    fprintf(out, "\t%%%d = fsub double -0.0, %%%d\n", result_reg, operand_val.reg_id);
            }
            else if (node->op == OP_UPLUS) {
                result_reg = operand_val.reg_id;
            }
            else {
                fprintf(stderr, "[COMPILATION ERROR] Operador unário não suportado.\n");
                exit(EXIT_FAILURE);
            }
            return create_llvm_value(result_reg, result_ast_type);
        }

        case AST_CALL: {
            Symbol *func_entry = lookup_symbol_in_scope(global_scope_name, node->id);
            if (!func_entry || (func_entry->category != SYMBOL_TYPE_FUNCTION && func_entry->category != SYMBOL_TYPE_PROCEDURE)) {
                fprintf(stderr, "[COMPILATION ERROR] Função '%s' não declarada, não é uma função, ou não retorna um valor.\n", node->id);
                exit(EXIT_FAILURE);
            }

            char *ret_type_str = get_llvm_type_str(func_entry->data_type);
            char args_str[1024] = "";

            ASTNode *current_arg_expression = node->left;
            int first_arg = 1;

            ASTNode *subprogram_def_ast_node = func_entry->ast_node_def;
            if (!subprogram_def_ast_node || (subprogram_def_ast_node->type != AST_FUNCTION && subprogram_def_ast_node->type != AST_PROCEDURE)) {
                fprintf(stderr, "[INTERNAL ERROR] O nó de definição da AST para '%s' é inválido ou está faltando para a chamada de função.\n", node->id);
                exit(EXIT_FAILURE);
            }
            ASTNode *current_param_def = subprogram_def_ast_node->left;

            while (current_arg_expression) {
                if (!current_param_def) {
                    fprintf(stderr, "[COMPILATION ERROR] Muitos argumentos fornecidos para a função '%s'.\n", node->id);
                    exit(EXIT_FAILURE);
                }
                if (current_param_def->type != AST_PARAM) {
                    fprintf(stderr, "[INTERNAL ERROR] Esperava-se um nó AST_PARAM na definição da função '%s', mas foi encontrado o tipo %d.\n", node->id, current_param_def->type);
                    exit(EXIT_FAILURE);
                }

                if (!first_arg)
                    strcat(args_str, ", ");
                first_arg = 0;

                char *param_llvm_type_str = get_llvm_type_str(current_param_def->data_type);
                
                if (current_param_def->is_reference) {
                    if (current_arg_expression->type != AST_VAR) {
                        fprintf(stderr, "[COMPILATION ERROR] Parâmetro de referência para a função '%s' espera uma variável, mas recebeu uma expressão (argumento: '%s').\n", node->id, current_arg_expression->id ? current_arg_expression->id : "anonymous expression");
                        exit(EXIT_FAILURE);
                    }
                    Symbol *arg_var_entry = lookup_symbol_in_scope(current_scope_name, current_arg_expression->id);
                    if (!arg_var_entry) {
                        arg_var_entry = lookup_symbol_in_scope(global_scope_name, current_arg_expression->id);
                        if (!arg_var_entry) {
                            fprintf(stderr, "[COMPILATION ERROR] Variável '%s' não encontrada para parâmetro de referência na chamada para '%s'.\n", current_arg_expression->id, node->id);
                            exit(EXIT_FAILURE);
                        }
                    }

                    if (arg_var_entry->category == SYMBOL_TYPE_VAR && lookup_symbol_in_scope(global_scope_name, current_arg_expression->id) == arg_var_entry) {
                        sprintf(args_str + strlen(args_str), "ptr noundef @%s", current_arg_expression->id);
                    } 
                    else if (arg_var_entry->category == SYMBOL_TYPE_VAR && arg_var_entry->is_reference) {
                        int loaded_ptr_reg = get_next_register();
                        fprintf(out, "\t%%%d = load ptr, ptr %%%s, align 8\n", loaded_ptr_reg, current_arg_expression->id);
                        sprintf(args_str + strlen(args_str), "ptr noundef %%%d", loaded_ptr_reg);
                    } 
                    else {
                        sprintf(args_str + strlen(args_str), "ptr noundef %%%s", current_arg_expression->id);
                    }
                } 
                else {
                    LLVMValue arg_val = generate_expression(current_arg_expression);
                    
                    if (arg_val.type != current_param_def->data_type) {
                        if (arg_val.type == TYPE_INTEGER && current_param_def->data_type == TYPE_REAL) {
                            int cast_reg = get_next_register();
                            fprintf(out, "\t%%%d = sitofp i32 %%%d to double\n", cast_reg, arg_val.reg_id);
                            arg_val.reg_id = cast_reg;
                            arg_val.type = TYPE_REAL;
                        } 
                        else if (arg_val.type == TYPE_REAL && current_param_def->data_type == TYPE_INTEGER) {
                            fprintf(stderr, "[SEMANTIC WARNING] Conversão implícita de REAL para INTEGER para argumento da função '%s'. Pode haver perda de dados.\n", node->id);
                            int cast_reg = get_next_register();
                            fprintf(out, "\t%%%d = fptosi double %%%d to i32\n", cast_reg, arg_val.reg_id);
                            arg_val.reg_id = cast_reg;
                            arg_val.type = TYPE_INTEGER;
                        } else {
                             fprintf(stderr, "[SEMANTIC ERROR] Incompatibilidade de tipo para o argumento da função '%s': Esperado %s, Recebido %s.\n",
                                        node->id, data_type_to_string(current_param_def->data_type), data_type_to_string(arg_val.type));
                             exit(EXIT_FAILURE);
                        }
                    }
                    sprintf(args_str + strlen(args_str), "%s noundef %%%d", get_llvm_type_str(arg_val.type), arg_val.reg_id);
                }
        
                current_arg_expression = current_arg_expression->next;
                current_param_def = current_param_def->next;
            }

            if (current_param_def) {
                fprintf(stderr, "[COMPILATION ERROR] Não foram fornecidos argumentos suficientes para a função '%s'.\n", node->id);
                exit(EXIT_FAILURE);
            }

            result_reg = -1;
            if (func_entry->data_type != TYPE_NONE) {
                result_reg = get_next_register();
                fprintf(out, "\t%%%d = call %s @%s(%s)\n", result_reg, ret_type_str, node->id, args_str);
                return create_llvm_value(result_reg, func_entry->data_type);
            } 
            else {
                fprintf(out, "\tcall void @%s(%s)\n", node->id, args_str);
                return create_llvm_value(-1, TYPE_NONE);
            }
        }

        default:
            fprintf(stderr, "[COMPILATION ERROR] Tipo de nó AST desconhecido (%d) em generate_expression.\n", node->type);
            exit(EXIT_FAILURE);
    }

    return create_llvm_value(-1, TYPE_NONE); 
}

char* generate_label_name(const char* prefix) {
    static int global_label_counter = 0;
    char buffer[256];
    
    if (strcmp(prefix, "if_then") == 0)
        sprintf(buffer, "%s_%d", prefix, if_counter);
    else if (strcmp(prefix, "if_else") == 0)
        sprintf(buffer, "%s_%d", prefix, if_counter);
    else if (strcmp(prefix, "if_end") == 0)
        sprintf(buffer, "%s_%d", prefix, if_counter);
    else if (strcmp(prefix, "while_cond") == 0)
        sprintf(buffer, "%s_%d", prefix, while_counter);
    else if (strcmp(prefix, "while_body") == 0)
        sprintf(buffer, "%s_%d", prefix, while_counter);
    else if (strcmp(prefix, "while_end") == 0)
        sprintf(buffer, "%s_%d", prefix, while_counter);
    else
        sprintf(buffer, "%s_%d", prefix, global_label_counter++);
    
    return strdup(buffer);
}

static void generate_statement(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case AST_ASSIGNMENT: {
            LLVMValue expr_val = generate_expression(node->right);

            Symbol *var_entry = lookup_symbol_in_scope(current_scope_name, node->left->id);
            int global = 0;
            if (!var_entry) {
                var_entry = lookup_symbol_in_scope(global_scope_name, node->left->id);
                if (!var_entry) {
                    fprintf(stderr, "[COMPILATION ERROR] Variável '%s' não encontrada para atribuição na função '%s' ou escopo global.\n", node->left->id, current_scope_name ? current_scope_name : "UNKNOWN");
                    exit(EXIT_FAILURE);
                }
                global = 1;
            }

            ASTDataType target_ast_type = var_entry->data_type;
            char *target_llvm_type_str = get_llvm_type_str(target_ast_type);
            int align = (target_ast_type == TYPE_INTEGER ? 4 : 8);

            int final_expr_reg_id = expr_val.reg_id;
            ASTDataType actual_expr_ast_type = expr_val.type;

            if (actual_expr_ast_type != target_ast_type) {
                if (target_ast_type == TYPE_REAL && actual_expr_ast_type == TYPE_INTEGER) {
                    int cast_reg = get_next_register();
                    fprintf(out, "\t%%%d = sitofp i32 %%%d to double\n", cast_reg, expr_val.reg_id);
                    final_expr_reg_id = cast_reg;
                    actual_expr_ast_type = TYPE_REAL;
                }
                else if (target_ast_type == TYPE_INTEGER && actual_expr_ast_type == TYPE_REAL) {
                    fprintf(stderr, "[SEMANTIC WARNING] Conversão implícita de REAL para INTEGER na atribuição de '%s'. Pode haver perda de dados.\n", node->left->id);
                    int cast_reg = get_next_register();
                    fprintf(out, "\t%%%d = fptosi double %%%d to i32\n", cast_reg, expr_val.reg_id);
                    final_expr_reg_id = cast_reg;
                    actual_expr_ast_type = TYPE_INTEGER;
                }
                else {
                     fprintf(stderr, "[COMPILATION ERROR] Tipos incompatíveis na atribuição após a coerção: Esperado %s, Recebido %s.\n",
                                data_type_to_string(target_ast_type), data_type_to_string(actual_expr_ast_type));
                     exit(EXIT_FAILURE);
                }
            }

            if (global) {
                fprintf(out, "\tstore %s %%%d, ptr @%s, align %d\n", target_llvm_type_str, final_expr_reg_id, node->left->id, align);
            }
            else if (var_entry->is_reference) {
                int ptr_reg = get_next_register();
                fprintf(out, "\t%%%d = load ptr, ptr %%%s, align 8\n", ptr_reg, node->left->id);
                fprintf(out, "\tstore %s %%%d, ptr %%%d, align %d\n", target_llvm_type_str, final_expr_reg_id, ptr_reg, align);
            }
            else {
                fprintf(out, "\tstore %s %%%d, ptr %%%s, align %d\n", target_llvm_type_str, final_expr_reg_id, node->left->id, align);
            }
            break;
        }

        case AST_READ: {
            ASTNode *current_var_to_read = node->left;
            while (current_var_to_read) {
                if (current_var_to_read->type != AST_VAR) {
                    fprintf(stderr, "[COMPILATION ERROR] A instrução READ espera uma variável, mas foi encontrado o tipo %d.\n", current_var_to_read->type);
                    exit(EXIT_FAILURE);
                }

                int global = 0;
                Symbol *var_entry = lookup_symbol_in_scope(current_scope_name, current_var_to_read->id);
                if (!var_entry) {
                    var_entry = lookup_symbol_in_scope(global_scope_name, current_var_to_read->id);
                    if (!var_entry) {
                        fprintf(stderr, "[COMPILATION ERROR] Variável '%s' não encontrada para a instrução READ.\n", current_var_to_read->id);
                        exit(EXIT_FAILURE);
                    }
                    global = 1;
                }

                char *llvm_type_str = get_llvm_type_str(var_entry->data_type);
                char *format_str_ptr = (var_entry->data_type == TYPE_INTEGER) ? "@read_int" : "@read_float";
                int scanf_ret_reg = get_next_register();

                if (global) {
                    fprintf(out, "\t%%%d = call i32 (ptr, ...) @__isoc99_scanf(ptr noundef %s, ptr @%s)\n", scanf_ret_reg, format_str_ptr, current_var_to_read->id);
                }
                else if (var_entry->is_reference) {
                    int ptr_reg = get_next_register();
                    fprintf(out, "\t%%%d = load ptr, ptr %%%s, align 8\n", ptr_reg, current_var_to_read->id);
                    fprintf(out, "\t%%%d = call i32 (ptr, ...) @__isoc99_scanf(ptr noundef %s, ptr %%%d)\n", scanf_ret_reg, format_str_ptr, ptr_reg);
                }
                else {
                    fprintf(out, "\t%%%d = call i32 (ptr, ...) @__isoc99_scanf(ptr noundef %s, ptr %%%s)\n", scanf_ret_reg, format_str_ptr, current_var_to_read->id);
                }
                current_var_to_read = current_var_to_read->next;
            }
            break;
        }

        case AST_WRITE: {
            LLVMValue expr_val = generate_expression(node->left);
            
            char *llvm_type_str = get_llvm_type_str(expr_val.type);
            char *format_str_ptr = (expr_val.type == TYPE_INTEGER) ? "@write_int" : "@write_float";
            int printf_ret_reg = get_next_register();

            fprintf(out, "\t%%%d = call i32 (ptr, ...) @printf(ptr noundef %s, %s noundef %%%d)\n", printf_ret_reg, format_str_ptr, llvm_type_str, expr_val.reg_id);
            break;
        }

        case AST_WRITELN: {
            int printf_ret_reg = get_next_register();
            fprintf(out, "\t%%%d = call i32 (ptr, ...) @printf(ptr noundef @.str_newline)\n", printf_ret_reg);
            break;
        }

        case AST_CALL: {
            generate_expression(node);
            break;
        }

        case AST_IF: {
            if_counter++;
            LLVMValue cond_val = generate_expression(node->left); 

            int bool_cond_reg = get_next_register();
            fprintf(out, "\t%%%d = icmp ne i32 %%%d, 0\n", bool_cond_reg, cond_val.reg_id);

            char *then_label_name = generate_label_name("if_then");
            char *else_label_name = generate_label_name("if_else");
            char *end_if_label_name = generate_label_name("if_end");

            ASTNode *if_block_node = node->right;

            ASTNode *then_block = if_block_node->left;
            ASTNode *else_block = if_block_node->right;

            if (else_block)
                fprintf(out, "\tbr i1 %%%d, label %%%s, label %%%s\n\n", bool_cond_reg, then_label_name, else_label_name);
            else
                fprintf(out, "\tbr i1 %%%d, label %%%s, label %%%s\n\n", bool_cond_reg, then_label_name, end_if_label_name);

            fprintf(out, "%s:\n", then_label_name);
            generate_statement(then_block);
            fprintf(out, "\tbr label %%%s\n\n", end_if_label_name);

            if (else_block) {
                fprintf(out, "%s:\n", else_label_name);
                generate_statement(else_block);
                fprintf(out, "\tbr label %%%s\n\n", end_if_label_name);
            }

            fprintf(out, "%s:\n", end_if_label_name);

            free(then_label_name);
            free(else_label_name);
            free(end_if_label_name);
            break;
        }

        case AST_WHILE: {
            while_counter++;

            char *loop_cond_label_name = generate_label_name("while_cond");
            char *loop_body_label_name = generate_label_name("while_body");
            char *end_loop_label_name = generate_label_name("while_end");

            fprintf(out, "\tbr label %%%s\n\n", loop_cond_label_name);

            fprintf(out, "%s:\n", loop_cond_label_name);
            LLVMValue cond_val = generate_expression(node->left);

            int bool_cond_reg = get_next_register();
            fprintf(out, "\t%%%d = icmp ne i32 %%%d, 0\n", bool_cond_reg, cond_val.reg_id);

            fprintf(out, "\tbr i1 %%%d, label %%%s, label %%%s\n\n", bool_cond_reg, loop_body_label_name, end_loop_label_name);

            fprintf(out, "%s:\n", loop_body_label_name);
            generate_statement(node->right);
            fprintf(out, "\tbr label %%%s\n\n", loop_cond_label_name);

            fprintf(out, "%s:\n", end_loop_label_name);

            free(loop_cond_label_name);
            free(loop_body_label_name);
            free(end_loop_label_name);
            break;
        }

        case AST_FUNCTION_BLOCK:
            if (node->right)
                generate_statement(node->right);
            break;

        default:
            fprintf(stderr, "[WARNING] Tipo de nó AST não tratado (%d) em generate_statement.\n", node->type);
            break;
    }

    generate_statement(node->next);
}

static void generate_subprogram(ASTNode *subprogram_node) {
    if (!subprogram_node || (subprogram_node->type != AST_FUNCTION && subprogram_node->type != AST_PROCEDURE)) {
        fprintf(stderr, "[WARNING] Esperava-se AST_FUNCTION ou AST_PROCEDURE, mas foi encontrado o tipo %d para o nó com ID '%s'.\n", subprogram_node ? subprogram_node->type : -1, subprogram_node ? subprogram_node->id : "NULL");
        return;
    }

    char *previous_scope_name = current_scope_name;
    current_scope_name = subprogram_node->id;

    char param_list_str[512] = "";
    char param_alloc_str[1024] = "";

    ASTNode *current_param_ast = subprogram_node->left; 
    int arg_idx = 0;

    int first_param = 1;
    while (current_param_ast) {
        if (current_param_ast->type == AST_PARAM) {
            char *llvm_type_str = get_llvm_type_str(current_param_ast->data_type);
            int align = (current_param_ast->data_type == TYPE_INTEGER ? 4 : 8);

            if (!first_param)
                strcat(param_list_str, ", ");
            first_param = 0;

            if (current_param_ast->is_reference) {
                sprintf(param_list_str + strlen(param_list_str), "ptr noundef %%%d", arg_idx);
                sprintf(param_alloc_str + strlen(param_alloc_str), "\t%%%s = alloca ptr, align 8\n", current_param_ast->id);
                sprintf(param_alloc_str + strlen(param_alloc_str), "\tstore ptr %%%d, ptr %%%s, align 8\n", arg_idx, current_param_ast->id);
            } 
            else {
                sprintf(param_list_str + strlen(param_list_str), "%s noundef %%%d", llvm_type_str, arg_idx);
                sprintf(param_alloc_str + strlen(param_alloc_str), "\t%%%s = alloca %s, align %d\n", current_param_ast->id, llvm_type_str, align);
                sprintf(param_alloc_str + strlen(param_alloc_str), "\tstore %s %%%d, ptr %%%s, align %d\n", llvm_type_str, arg_idx, current_param_ast->id, align);
            }
            arg_idx++;
        } 
        else {
            fprintf(stderr, "[WARNING] Esperava-se um nó AST_PARAM para o parâmetro no subprograma '%s', mas foi encontrado o tipo %d.\n", subprogram_node->id, current_param_ast->type);
        }
        current_param_ast = current_param_ast->next;
    }

    ASTDataType return_ast_type = TYPE_NONE;
    char *llvm_return_type_str = "void"; 

    if (subprogram_node->type == AST_FUNCTION) {
        return_ast_type = subprogram_node->data_type;
        llvm_return_type_str = get_llvm_type_str(return_ast_type);
    }

    char *subprogram_name = subprogram_node->id;

    fprintf(out, "define %s @%s(%s) {\n", llvm_return_type_str, subprogram_name, param_list_str);
    fprintf(out, "entry:\n");
    fprintf(out, "%s", param_alloc_str);

    register_counter = arg_idx;
    ASTNode *function_block = subprogram_node->right;
    if (function_block && function_block->type == AST_FUNCTION_BLOCK) {
        if (function_block->left && function_block->left->type == AST_DECLARATIONS) 
            generate_local_allocations(function_block->left);

        if (subprogram_node->type == AST_FUNCTION) {
            char *llvm_type_str_ret = get_llvm_type_str(return_ast_type);
            int align_ret = (return_ast_type == TYPE_INTEGER ? 4 : 8);
            fprintf(out, "\t%%%s = alloca %s, align %d\n", subprogram_name, llvm_type_str_ret, align_ret);
        }
        
        if (function_block->right)
            generate_statement(function_block->right);
    } 
    else {
        fprintf(stderr, "[WARNING] Esperava-se AST_FUNCTION_BLOCK para o subprograma '%s', mas foi encontrado o tipo %d.\n", subprogram_name, function_block ? function_block->type : -1);
    }
    
    if (subprogram_node->type == AST_FUNCTION) {
        char *llvm_type_str_ret = get_llvm_type_str(return_ast_type);
        int align_ret = (return_ast_type == TYPE_INTEGER ? 4 : 8);
        int ret_val_reg = get_next_register();
        fprintf(out, "\t%%%d = load %s, ptr %%%s, align %d\n", ret_val_reg, llvm_type_str_ret, subprogram_name, align_ret);
        fprintf(out, "\tret %s %%%d\n", llvm_type_str_ret, ret_val_reg);
    } 
    else {
        fprintf(out, "\tret void\n");
    }

    fprintf(out, "}\n\n");
    current_scope_name = previous_scope_name;
}

void codegen(ASTNode *root, const char *output_filename) {
    out = fopen(output_filename, "w");
    if (!out) {
        fprintf(stderr, "[ERROR] Não foi possível abrir o arquivo de saída '%s'.\n", output_filename);
        exit(EXIT_FAILURE);
    }

    perform_semantic_analysis(root);

    fprintf(out,
        "; Declarações de funções padrão C para I/O\n"
        "declare i32 @printf(ptr noundef, ...)\n"
        "declare i32 @__isoc99_scanf(ptr noundef, ...)\n"
        "@read_int = private unnamed_addr constant [3 x i8] c\"%%d\\00\", align 1\n"
        "@write_int = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", align 1\n"
        "@read_float = private unnamed_addr constant [3 x i8] c\"%%f\\00\", align 1\n"
        "@write_float = private unnamed_addr constant [4 x i8] c\"%%f\\0A\\00\", align 1\n"
        "@.str_newline = private unnamed_addr constant [2 x i8] c\"\\0A\\00\", align 1\n"
    );

    global_scope_name = root->id;
    current_scope_name = NULL;

    fprintf(out, "\n; Declarações de variáveis globais\n");
    if (root->left && root->left->type == AST_DECLARATIONS)
        generate_global_declarations(root->left);

    fprintf(out, "\n; Definições de funções e procedimentos\n");
    ASTNode *current_subprogram = root->right;
    while (current_subprogram) {
        generate_subprogram(current_subprogram);
        current_subprogram = current_subprogram->next;
    }

    register_counter = 0;

    fprintf(out, "; Função main do programa\n");
    fprintf(out, "define i32 @main() {\n");
    fprintf(out, "entry:\n");
    
    if (root->next)
        generate_statement(root->next);
    
    fprintf(out, "\tret i32 0\n");
    fprintf(out, "}\n");

    fclose(out);
}