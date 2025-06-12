#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codegen.h"
#include "symbols_table.h"

static FILE *out;

void codegen(ASTNode *root, const char *output_filename) {
    out = fopen(output_filename, "w");
    if (!out) {
        perror("fopen");
        exit(1);
    }

    fprintf(out,
        "declare i32 @printf(ptr noundef, ...)\n"
        "declare i32 @__isoc99_scanf(ptr noundef, ...)\n"
        "@read_int = private unnamed_addr constant [3 x i8] c\"%d\\00\", align 1\n"
        "@write_int = private unnamed_addr constant [4 x i8] c\"%d\\0A\\00\", align 1\n"
        "@read_float = private unnamed_addr constant [3 x i8] c\"%f\\00\", align 1\n"
        "@write_float = private unnamed_addr constant [4 x i8] c\"%f\\0A\\00\", align 1\n"
    );

    codegen_declarations(root->left);

    fprintf(out, "define i32 @main() {\n");
    fprintf(out, "  ret i32 0\n");
    fprintf(out, "}\n");

    fclose(out);
}
