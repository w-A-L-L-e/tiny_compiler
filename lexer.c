#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

const char *keywords[] = {"if", "else", "while", "return", "int", "void", NULL};
const char *operators[] = {"+", "-", "*", "/", "=", "==", "!=", "<", "<=", ">", ">=", NULL};
const char *punctuation = ";(){},";

int is_keyword(const char *str) {
    for (int i = 0; keywords[i]; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int is_operator_char(char c) {
    return strchr("+-*/=<>!", c) != NULL;
}

Token next_token(const char **src) {
    while (isspace(**src)) (*src)++;

    if (**src == '\0')
        return (Token){TOKEN_EOF, ""};

    Token token;
    token.value[0] = '\0';

    if (isalpha(**src) || **src == '_') {
        int len = 0;
        while (isalnum(**src) || **src == '_')
            token.value[len++] = *(*src)++;
        token.value[len] = '\0';

        token.type = is_keyword(token.value) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
    } else if (isdigit(**src)) {
        int len = 0;
        while (isdigit(**src))
            token.value[len++] = *(*src)++;
        token.value[len] = '\0';
        token.type = TOKEN_NUMBER;
    } else if (is_operator_char(**src)) {
        int len = 0;
        token.value[len++] = *(*src)++;
        if ((token.value[0] == '=' || token.value[0] == '!' ||
             token.value[0] == '<' || token.value[0] == '>') && **src == '=') {
            token.value[len++] = *(*src)++;
        }
        token.value[len] = '\0';
        token.type = TOKEN_OPERATOR;
    } else if (strchr(punctuation, **src)) {
        token.value[0] = *(*src)++;
        token.value[1] = '\0';
        token.type = TOKEN_PUNCTUATION;
    } else {
        token.value[0] = *(*src)++;
        token.value[1] = '\0';
        token.type = TOKEN_UNKNOWN;
    }

    return token;
}

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

// int main() {
//     const char *source_code = "int x = 42; if (x > 10) { return x; }";
//     const char *src = source_code;
// 
//     Token token;
//     do {
//         token = next_token(&src);
//         printf("%-12s : %s\n", token_type_name(token.type), token.value);
//     } while (token.type != TOKEN_EOF);
// 
//     return 0;
// }
