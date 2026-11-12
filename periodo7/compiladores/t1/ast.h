#ifndef AST_H
#define AST_H

typedef enum {
    AST_VAR,
    AST_NUM,
    AST_BINOP,
    AST_UNOP,
    AST_CALL,
    AST_PROG,
    AST_READ,
    AST_WRITE,
    AST_WRITELN,
    AST_IF,
    AST_IF_BLOCK,
    AST_WHILE,
    AST_FUNCTION,
    AST_PROCEDURE,
    AST_FUNCTION_BLOCK,
    AST_PARAM,
    AST_ARG_LIST,
    AST_DECLARATIONS,
    AST_ASSIGNMENT,
} ASTNodeType;

typedef enum {
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_NONE
} ASTDataType;

typedef enum {
    OP_EQ,
    OP_NE,
    OP_LT,
    OP_LE,
    OP_GT,
    OP_GE,
    OP_PLUS,
    OP_MINUS,
    OP_OR,
    OP_MUL,
    OP_DIV,
    OP_DIV_INT,
    OP_MOD,
    OP_AND,
    OP_UMINUS,
    OP_UPLUS
} ASTOperator;

typedef struct ASTNode {
    ASTNodeType type;
    struct ASTNode *left;
    struct ASTNode *right;
    char *id;
    double value;
    ASTOperator op;
    int is_reference;
    ASTDataType data_type;
    struct ASTNode *next;
} ASTNode;

ASTNode *create_node(ASTNodeType type, ASTNode *left, ASTNode *right);
ASTNode *create_num(int value);
ASTNode *create_var(char *id);
ASTNode *create_read(ASTNode *id_list);
ASTNode *create_write(ASTNode *expr);
ASTNode *create_binop(ASTOperator op, ASTNode *left, ASTNode *right);
ASTNode *create_unop(ASTOperator op, ASTNode *operand);
ASTNode *create_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch);
ASTNode *create_while(ASTNode *cond, ASTNode *body);
ASTNode *create_call(char *id, ASTNode *args);
ASTNode *create_param(char *id, ASTDataType type, int is_ref);
ASTNode *create_arg_list(ASTNode *arg);
ASTNode *create_assignment(ASTNode *var, ASTNode *expr);
ASTNode *create_function_block(ASTNode *declarations, ASTNode *body); 
ASTNode *create_function(char *id, ASTNode *params, ASTDataType return_type, ASTNode *block);
ASTNode *create_procedure(char *id, ASTNode *params, ASTNode *block);
ASTNode *create_program(char *id, ASTNode *global_decls, ASTNode *subprograms, ASTNode *main_body);

const char* data_type_to_string(ASTDataType type);
void print_ast(ASTNode *node, int indent_level);
void free_ast(ASTNode *node);
ASTDataType perform_semantic_analysis(ASTNode *node);

#endif // AST_H
