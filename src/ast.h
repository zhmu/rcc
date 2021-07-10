#ifndef AST_H
#define AST_H

enum AST_NODE_TYPE {
    AN_ADD,
    AN_SUBTRACT,
    AN_MULTIPLY,
    AN_DIVIDE,
    AN_LITERAL,
};

union AST_VALUE {
    int int_value;
};

struct AST_NODE {
    enum AST_NODE_TYPE type;
    struct AST_NODE* left;
    struct AST_NODE* right;
    union AST_VALUE value;
};

struct AST_NODE* ast_new_binary_node(enum AST_NODE_TYPE type, struct AST_NODE* left, struct AST_NODE* right, union AST_VALUE value);
struct AST_NODE* ast_new_leaf_node(enum AST_NODE_TYPE type, union AST_VALUE value);
struct AST_NODE* ast_new_unary_node(enum AST_NODE_TYPE type, struct AST_NODE* left, union AST_VALUE value);

#endif
