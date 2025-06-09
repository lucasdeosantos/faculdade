#ifndef AST_H
#define AST_H

typedef enum {
    AST_VAR,
    AST_NUM,
    AST_BINOP,
    AST_UNOP,
    AST_CALL,
    AST_STATEMENT,
    AST_PROG,
    AST_READ,
    AST_WRITE,
    AST_WRITELN,
    AST_IF,
    AST_WHILE,
    AST_FUNCTION,
    AST_PROCEDURE,
    AST_PARAM,
    AST_ARG_LIST,
    AST_DECLARATIONS
} ASTNodeType;

typedef enum {
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_NONE
} ASTDataType;

typedef struct ASTNode {
    ASTNodeType type;
    struct ASTNode *left;
    struct ASTNode *right;
    char *id;               // Nome da variável/função
    double value;           // Valor numérico
    int op;                 // Operador (ex: '+', '-', etc.)
    int is_reference;       // Indica se parâmetro é por referência
    ASTDataType data_type;  // Tipo de dado: int ou real
    struct ASTNode *next;
} ASTNode;

ASTNode *create_node(ASTNodeType type, ASTNode *left, ASTNode *right);
ASTNode *create_num(double value);
ASTNode *create_var(char *id);
ASTNode *create_read(char *id);
ASTNode *create_write(ASTNode *expr);
ASTNode *create_binop(int op, ASTNode *left, ASTNode *right);
ASTNode *create_unop(int op, ASTNode *operand);
ASTNode *create_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch);
ASTNode *create_while(ASTNode *cond, ASTNode *body);
ASTNode *create_call(char *id, ASTNode *args);
ASTNode *create_param(char *id, ASTDataType type, int is_ref);
ASTNode *create_arg_list(ASTNode *arg);

void print_ast(ASTNode *node);
void free_ast(ASTNode *node);

#endif // AST_H