#include "expr.h"
#include "ast.h"
#include "scan.h"
#include "lib.h"

// Higher value indicate a higher precedence
static int get_token_operator_precedence(enum TOKEN_TYPE tt)
{
    switch(tt) {
        case TT_PLUS:
        case TT_MINUS:
            return 1;
        case TT_STAR:
        case TT_SLASH:
            return 2;
    }
    fatal("syntax error: unexpected token type %d", tt);
}

static enum AST_NODE_TYPE token_type_to_ast_node_type(enum TOKEN_TYPE tt)
{
    switch(tt) {
        case TT_PLUS: return AN_ADD;
        case TT_MINUS: return AN_SUBTRACT;
        case TT_STAR: return AN_MULTIPLY;
        case TT_SLASH: return AN_DIVIDE;
    }
    fatal("token_type_to_ast_node_type(): unrecognized token %d", tt);
}

static struct AST_NODE* primary_expr(struct Token* t)
{
    struct AST_NODE* n;
    union AST_VALUE v;

    switch(t->type) {
        case TT_LITERAL_INT:
            v.int_value = t->value;
            n = ast_new_leaf_node(AN_LITERAL, v);

            scan(t); // get the next token
            return n;
    }

    fatal("primary_expr: syntax error (token %d unrecognized here)", t->type);
}

struct AST_NODE* binary_expr(struct Token* t, const int previous_token_precedence)
{
    struct AST_NODE* left = primary_expr(t); // updates t
    enum TOKEN_TYPE token_type = t->type;
    if (token_type == TT_EOF)
        return left;

    while(get_token_operator_precedence(token_type) > previous_token_precedence) {
        scan(t);

        const int right_prec = get_token_operator_precedence(token_type);
        struct AST_NODE* right = binary_expr(t, right_prec);

        const int node_type = token_type_to_ast_node_type(token_type);
        left = ast_new_binary_node(node_type, left, right, ast_no_value);

        token_type = t->type;
        if (token_type == TT_EOF)
            return left;
    }

    return left;
}
