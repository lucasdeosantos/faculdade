#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

void codegen(ASTNode *root, const char* output_filename);

#endif // CODEGEN_H