#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

typedef struct {
    int reg_id;
    ASTDataType type;
} LLVMValue;

void codegen(ASTNode *root, const char* output_filename);

#endif // CODEGEN_H