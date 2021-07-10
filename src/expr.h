#ifndef EXPR_H
#define EXPR_H

struct Token;

#ifdef __cplusplus
extern "C" {
#endif

struct AST_NODE* binary_expr(struct Token* t, int previous_token_precedence);

#ifdef __cplusplus
}
#endif

#endif
