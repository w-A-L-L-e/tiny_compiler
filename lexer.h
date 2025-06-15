typedef enum {
    TOKEN_KEYWORD, TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_OPERATOR,
    TOKEN_PUNCTUATION, TOKEN_EOF, TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[100];
} Token;

int is_keyword(const char *str);
int is_operator_char(char c);
Token next_token(const char **src);
const char* token_type_name(TokenType type);


