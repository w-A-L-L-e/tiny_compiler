#include "parser.h"
#include "compiler.h"

#include <stdio.h>
#include <string.h>

int main() {
    // TODO: implement else support in parser and compiler here
    // const char *code = "int x; x = 42; if (x) { return 0; } else {return 42;}";
    const char *code = "int x; x = 42; if (x) { return 22; }";
    ASTNode *program = parse_program(code);

    FILE *out = fopen("output.asm", "w");
    if (!out) {
        perror("fopen");
        return 1;
    }

    compile_program(program, out);
    fclose(out);

    printf("Assembly written to output.asm\n");
    return 0;
}
