#include "ast_helper.h"
#include <sstream>
#include <iostream>

std::string format_ast_node(struct AST_NODE* n)
{
    using namespace std::string_literals;
    switch(n->type) {
        case AN_ADD:
            return "add";
        case AN_SUBTRACT:
            return "subtract";
        case AN_MULTIPLY:
            return "multiply";
        case AN_DIVIDE:
            return "divide";
        case AN_LITERAL:
            return "literal "s + std::to_string(n->value.int_value);
    }
    return "?? type "s + std::to_string(n->type);
}

std::string ast_to_string(struct AST_NODE* n)
{
    std::ostringstream s;
    s << "(" << format_ast_node(n);
    if (n->left) {
        s << " " << ast_to_string(n->left);
    } else {
        s << " nil";
    }
    if (n->right) {
        s << " " << ast_to_string(n->right);
    } else {
        s << " nil";
    }
    s << ")";
    return s.str();
}

// https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
void dump_ast(const std::string& prefix, struct AST_NODE* n, bool isLeft)
{
    if (n == nullptr) return;

    std::cout << prefix;
    std::cout << (isLeft ? "|--" : "+--" );

    std::cout << format_ast_node(n);
    std::cout << "\n";

    // enter the next tree level - left and right branch
    dump_ast( prefix + (isLeft ? "|   " : "    "), n->left, true);
    dump_ast( prefix + (isLeft ? "|   " : "    "), n->right, false);
}

void dump_ast(struct AST_NODE* n)
{
    dump_ast("", n, false);
}

