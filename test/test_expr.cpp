#include <gtest/gtest.h>

#include "expr.h"
#include "ast.h"

#include "ast_helper.h"
#include "scanner_feeder.h"
#include "fatal_guard.h"

namespace {

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
