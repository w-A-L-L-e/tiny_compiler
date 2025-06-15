#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"  // use the lexer code from before and export `Token`, `next_token`, etc.
#include "parser.h"

// Global current token
Token current_token;
const char *src;

void advance() {
    current_token = next_token(&src);
}

ASTNode *new_node(ASTNodeType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    node->left = node->right = node->condition = NULL;
    node->body = NULL;
    node->body_count = 0;
    node->value[0] = '\0';
    return node;
}

ASTNode *parse_expression() {
    ASTNode *node = NULL;
    if (current_token.type == TOKEN_IDENTIFIER) {
        node = new_node(AST_EXPR_IDENTIFIER);
        strcpy(node->value, current_token.value);
        advance();
    } else if (current_token.type == TOKEN_NUMBER) {
        node = new_node(AST_EXPR_NUMBER);
        strcpy(node->value, current_token.value);
        advance();
    }
    return node;
}

ASTNode *parse_declaration() {
    advance(); // skip 'int'
    if (current_token.type != TOKEN_IDENTIFIER) return NULL;
    ASTNode *node = new_node(AST_DECLARATION);
    strcpy(node->value, current_token.value);
    advance();
    if (strcmp(current_token.value, ";") != 0) return NULL;
    advance();
    return node;
}

ASTNode *parse_assignment() {
    ASTNode *node = new_node(AST_ASSIGNMENT);
    strcpy(node->value, current_token.value);
    advance();
    if (strcmp(current_token.value, "=") != 0) return NULL;
    advance();
    node->right = parse_expression();
    if (strcmp(current_token.value, ";") != 0) return NULL;
    advance();
    return node;
}

ASTNode *parse_return() {
    advance(); // skip 'return'
    ASTNode *node = new_node(AST_RETURN);
    node->left = parse_expression();
    if (strcmp(current_token.value, ";") != 0) return NULL;
    advance();
    return node;
}

//ASTNode *parse_statement();

ASTNode* parse_block() {
  ASTNode* node = new_node(AST_BLOCK);
  return node;
}

ASTNode *parse_if_statement() {
    advance(); // skip 'if'
    if (strcmp(current_token.value, "(") != 0) return NULL;
    advance();
    ASTNode *node = new_node(AST_IF);
    node->condition = parse_expression();
    if (strcmp(current_token.value, ")") != 0) return NULL;
    advance();
  
    // TODO: refactor to use block and also make support
    // form else part of if statement
    // 
    if (strcmp(current_token.value, "{") != 0) return NULL;
    advance();

    node->body = malloc(sizeof(ASTNode*) * 100);
    node->body_count = 0;
    while (strcmp(current_token.value, "}") != 0 && current_token.type != TOKEN_EOF) {
        ASTNode *stmt = parse_statement();
        if (!stmt) return NULL;
        node->body[node->body_count++] = stmt;
    }
    if (strcmp(current_token.value, "}") != 0) return NULL;
    advance();
    return node;
}

ASTNode *parse_statement() {
    if (strcmp(current_token.value, "int") == 0) {
        return parse_declaration();
    } else if (strcmp(current_token.value, "return") == 0) {
        return parse_return();
    } else if (strcmp(current_token.value, "if") == 0) {
        return parse_if_statement();
    } else if (current_token.type == TOKEN_IDENTIFIER) {
        return parse_assignment();
    }
    return NULL;
}



ASTNode *parse_program(const char *source_code) {
    src = source_code;
    advance();

    ASTNode *program = new_node(AST_PROGRAM);
    program->body = malloc(sizeof(ASTNode*) * 100);
    program->body_count = 0;

    while (current_token.type != TOKEN_EOF) {
        ASTNode *stmt = parse_statement();
        if (!stmt) {
            printf("Parse error\n");
            exit(1);
        }
        program->body[program->body_count++] = stmt;
    }

    return program;
}


void print_ast(ASTNode *node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) printf("  ");
    switch (node->type) {
        case AST_PROGRAM:     
            printf("Program\n"); 
            for (int i = 0; i < node->body_count; i++)
              print_ast(node->body[i], indent + 2);
          break;
        case AST_DECLARATION: printf("Declaration: %s\n", node->value); break;
        case AST_ASSIGNMENT:  printf("Assignment: %s = \n", node->value); print_ast(node->right, indent + 1); return;
        case AST_RETURN:      printf("Return\n"); print_ast(node->left, indent + 1); return;
        case AST_IF:
            printf("If Statement\n");
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Condition:\n");
            print_ast(node->condition, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("  ");
            printf("Body:\n");
            for (int i = 0; i < node->body_count; i++)
                print_ast(node->body[i], indent + 2);
            return;
        case AST_EXPR_IDENTIFIER: printf("Identifier: %s\n", node->value); break;
        case AST_EXPR_NUMBER:     printf("Number: %s\n", node->value); break;
        default: printf("Unknown Node\n");
    }
}

//int main() {
//     const char *code = "int x; x = 42; if (x) { return x; }";
//    ASTNode *program = parse_program(code);
//    print_ast(program, 0);
//    return 0;
//}
