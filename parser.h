
typedef enum {
    AST_PROGRAM, AST_DECLARATION, AST_ASSIGNMENT, AST_BLOCK,
    AST_IF, AST_RETURN, AST_EXPR_IDENTIFIER, AST_EXPR_NUMBER
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *condition;
    struct ASTNode **body;
    int body_count;
    char value[100];
} ASTNode;

// functions used by our recursive descent parser
ASTNode *new_node(ASTNodeType type);
ASTNode *parse_expression();
ASTNode *parse_declaration();
ASTNode *parse_assignment();
ASTNode *parse_return();
ASTNode *parse_statement();
ASTNode *parse_if_statement();

// call this on program text
ASTNode *parse_program(const char *source_code);

// use to pretty print the Abstract Syntax Tree
void print_ast(ASTNode *node, int indent);

