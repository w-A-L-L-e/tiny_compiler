#include <string.h>
#include <stdio.h>
#include "parser.h" // contains ASTNode definition and parse_program()
#include "compiler.h"

// Very simple symbol table to track variables and stack offsets
typedef struct {
    char name[100];
    int offset;  // offset from ebp
} Symbol;

Symbol symbols[100];
int symbol_count = 0;
int current_offset = -4; // local variables stored at negative offsets

int lookup_variable(const char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbols[i].name, name) == 0)
            return symbols[i].offset;
    }
    // Not found, declare new
    strcpy(symbols[symbol_count].name, name);
    symbols[symbol_count].offset = current_offset;
    current_offset -= 4;
    return symbols[symbol_count++].offset;
}

void generate_expression(ASTNode *node, FILE *out) {
    if (node->type == AST_EXPR_NUMBER) {
        fprintf(out, "    mov eax, %s\n", node->value);
    } else if (node->type == AST_EXPR_IDENTIFIER) {
        int offset = lookup_variable(node->value);
        fprintf(out, "    mov eax, [ebp%+d]\n", offset);
    }
}


void generate_if(ASTNode *node, FILE *out, int *label_counter) {
    int label_id = (*label_counter)++;
    generate_expression(node->condition, out);
    fprintf(out, "    cmp eax, 0\n");
    fprintf(out, "    je else_%d\n", label_id);

    for (int i = 0; i < node->body_count; i++)
        generate_statement(node->body[i], out);

    fprintf(out, "else_%d:\n", label_id);
}

void generate_statement(ASTNode *node, FILE *out) {
    switch (node->type) {
        case AST_DECLARATION:
            lookup_variable(node->value); // register variable
            break;
        case AST_ASSIGNMENT: {
            generate_expression(node->right, out);
            int offset = lookup_variable(node->value);
            fprintf(out, "    mov [ebp%+d], eax\n", offset);
            break;
        }
        case AST_RETURN:
            generate_expression(node->left, out);
            fprintf(out, "    mov ebx, eax\n");
            fprintf(out, "    jmp program_end\n");
            break;
        case AST_IF:
            static int label_counter = 0;
            generate_if(node, out, &label_counter);
            break;
        default:
            fprintf(out, "    ; Unknown statement\n");
    }
}



void compile_program(ASTNode *program, FILE *out) {
    fprintf(out,
        "format ELF executable 3\n"
        "entry start\n\n"

        "segment readable writable\n"
    );

    // Variable declarations
    for (int i = 0; i < program->body_count; i++) {
        ASTNode *stmt = program->body[i];
        if (stmt->type == AST_DECLARATION) {
            fprintf(out, "%s dd 0\n", stmt->value);
        }
    }

    fprintf(out,
        "\nsegment readable executable\n"
        "start:\n"

        "    ; Set up the stack frame\n"
        "    push ebp         ; Save the old base pointer\n"
        "    mov ebp, esp     ; Set the new base pointer\n"
    );

    // Statements
    for (int i = 0; i < program->body_count; i++) {
        ASTNode *stmt = program->body[i];
        if (stmt->type != AST_DECLARATION) {
            generate_statement(stmt, out);
        }
    }

    // Global label for return
    fprintf(out,
        "program_end:\n"
        "    mov eax, 1\n"
        "    int 0x80 \n"
    );
}


