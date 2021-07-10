#include "ast.h"
#include <assert.h>
#include <stdlib.h>

struct AST_NODE* ast_new_binary_node(enum AST_NODE_TYPE type, struct AST_NODE* left, struct AST_NODE* right, union AST_VALUE value)
{
    struct AST_NODE* n = malloc(sizeof(struct AST_NODE));
    assert(n != NULL);

    n->type = type;
    n->left = left;
    n->right = right;
    n->value = value;
    return n;
}

struct AST_NODE* ast_new_leaf_node(enum AST_NODE_TYPE type, union AST_VALUE value)
{
    return ast_new_binary_node(type, NULL, NULL, value);
}

struct AST_NODE* ast_new_unary_node(enum AST_NODE_TYPE type, struct AST_NODE* left, union AST_VALUE value)
{
    return ast_new_binary_node(type, left, NULL, value);
}
