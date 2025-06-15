#include <stdio.h>

int lookup_variable(const char *name);
void generate_expression(ASTNode *node, FILE *out);
void generate_statement(ASTNode *node, FILE *out);
void generate_if(ASTNode *node, FILE *out, int *label_counter);
void compile_program(ASTNode *program, FILE *out);
