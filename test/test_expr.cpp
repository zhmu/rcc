#include <gtest/gtest.h>

#include "expr.h"
#include "ast.h"

#include "scanner_feeder.h"
#include "fatal_guard.h"

using namespace std::string_literals;

namespace {

std::string format_ast_node(struct AST_NODE* n)
{
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

void VerifyExpressionAST(std::string_view sv, std::string_view expected_ast)
{
    ScannerFeeder s{std::string{sv}};

    Token t{};
    EXPECT_TRUE(scan(&t));

    auto n = binary_expr(&t, 0);
    ASSERT_TRUE(n);
    auto ast = ast_to_string(n);
    EXPECT_EQ(expected_ast, ast);
    if (expected_ast != ast) {
        dump_ast(n);
    }
}

}

TEST(Expr, Expression_1_plus_2)
{
    /*
     *   (+)
     *  /   \
     * 1     2
     */
    VerifyExpressionAST("1+2", "(add (literal 1 nil nil) (literal 2 nil nil))");
}

TEST(Expr, Expression_1_plus_2_minus_3)
{
    /*      (-)
     *     /   \
     *   (+)    3
     *  /   \
     * 1     2
     */
    VerifyExpressionAST("1+2-3", "(subtract (add (literal 1 nil nil) (literal 2 nil nil)) (literal 3 nil nil))");
}

TEST(Expr, Expression_1_times_2_minus_3_divide_4)
{
    /*
     *       (-)
     *      /   \
     *   (*)    (/)
     *  /   \   /  \
     * 1     2 3    4
     */
    VerifyExpressionAST("1*2-3/4", "(subtract (multiply (literal 1 nil nil) (literal 2 nil nil)) (divide (literal 3 nil nil) (literal 4 nil nil)))");
}
