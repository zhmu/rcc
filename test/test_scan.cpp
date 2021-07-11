#include <gtest/gtest.h>
#include <array>
#include <string_view>

#include "scanner_feeder.h"
#include "fatal_guard.h"

namespace {

constexpr auto MakeToken(TOKEN_TYPE type, int value = 0)
{
    return Token{type, value};
}

template<size_t N> void VerifySequence(std::string_view sv, const std::array<Token, N>& expected)
{
    ScannerFeeder s{std::string(sv)};
    for(auto e: expected) {
        Token t{};
        ASSERT_TRUE(scan(&t));
        EXPECT_EQ(e.type, t.type);
        EXPECT_EQ(e.value, t.value);
    }
    Token t{};
    EXPECT_FALSE(scan(&t));
}

void VerifyScanSingleToken(std::string_view sv, TOKEN_TYPE expected_token)
{
    VerifySequence(sv, std::array{ MakeToken(expected_token) });
}

void VerifyScanInteger(int value)
{
    VerifySequence(std::to_string(value), std::array{ MakeToken(TT_LITERAL_INT, value) });
}

void VerifyScanToken(std::string_view sv, KEYWORD keyword)
{
    VerifySequence(sv, std::array{ MakeToken(TT_KEYWORD, keyword) });
}
}

TEST(Scan, Empty)
{
    VerifySequence("", std::array<Token, 0>{ });
}

TEST(Scan, Unrecognized_Character) {
    FatalGuard fh;
    EXPECT_THROW(VerifyScanSingleToken("`", static_cast<TOKEN_TYPE>(0)), FatalError);
    EXPECT_EQ("unrecognized character '`' on line 1", fh.fatal_msg);
}

TEST(Scan, Plus) { VerifyScanSingleToken("+", TT_PLUS); }
TEST(Scan, Minus) { VerifyScanSingleToken("-", TT_MINUS); }
TEST(Scan, Star) { VerifyScanSingleToken("*", TT_STAR); }
TEST(Scan, Slash) { VerifyScanSingleToken("/", TT_SLASH); }

TEST(Scan, Integer_0) { VerifyScanInteger(0); }
TEST(Scan, Integer_1) { VerifyScanInteger(1); }
TEST(Scan, Integer_1234) { VerifyScanInteger(1234); }

TEST(Scan, Sequence) {
    VerifySequence("1+2*3-4/5", std::array{
        MakeToken(TT_LITERAL_INT, 1),
        MakeToken(TT_PLUS),
        MakeToken(TT_LITERAL_INT, 2),
        MakeToken(TT_STAR),
        MakeToken(TT_LITERAL_INT, 3),
        MakeToken(TT_MINUS),
        MakeToken(TT_LITERAL_INT, 4),
        MakeToken(TT_SLASH),
        MakeToken(TT_LITERAL_INT, 5),
    });
}

TEST(Scan, Unrecognized_Token) {
    FatalGuard fh;
    EXPECT_THROW(VerifySequence("foo", std::array<Token, 0>{ }), FatalError);
    EXPECT_EQ("unrecognized keyword 'foo'", fh.fatal_msg);
}

TEST(Scan, Token_Keywords)
{
    using KeywordValue = std::pair<std::string_view, KEYWORD>;
    constexpr std::array keywords = {
        KeywordValue{ "auto", K_AUTO },
        KeywordValue{ "break", K_BREAK },
        KeywordValue{ "case", K_CASE },
        KeywordValue{ "char", K_CHAR },
        KeywordValue{ "const", K_CONST },
        KeywordValue{ "continue", K_CONTINUE },
        KeywordValue{ "default", K_DEFAULT },
        KeywordValue{ "do", K_DO },
        KeywordValue{ "double", K_DOUBLE },
        KeywordValue{ "else", K_ELSE },
        KeywordValue{ "enum", K_ENUM },
        KeywordValue{ "extern", K_EXTERN },
        KeywordValue{ "float", K_FLOAT },
        KeywordValue{ "for", K_FOR },
        KeywordValue{ "goto", K_GOTO },
        KeywordValue{ "if", K_IF },
        KeywordValue{ "int", K_INT },
        KeywordValue{ "long", K_LONG },
        KeywordValue{ "register", K_REGISTER },
        KeywordValue{ "return", K_RETURN },
        KeywordValue{ "short", K_SHORT },
        KeywordValue{ "signed", K_SIGNED },
        KeywordValue{ "sizeof", K_SIZEOF },
        KeywordValue{ "static", K_STATIC },
        KeywordValue{ "struct", K_STRUCT },
        KeywordValue{ "switch", K_SWITCH },
        KeywordValue{ "typedef", K_TYPEDEF },
        KeywordValue{ "union", K_UNION },
        KeywordValue{ "unsigned", K_UNSIGNED },
        KeywordValue{ "void", K_VOID },
        KeywordValue{ "volatile", K_VOLATILE },
        KeywordValue{ "while", K_WHILE },
    };

    for(const auto& keyword: keywords) {
        VerifyScanToken(keyword.first, keyword.second);
    }
}

TEST(Scan, Tokens)
{
    using TokenValue = std::pair<std::string_view, TOKEN_TYPE>;
    constexpr std::array tokens = {
        TokenValue{ "...", TT_ELLIPSIS },
        TokenValue{ ">>=", TT_SHIFT_RIGHT_ASSIGN },
        TokenValue{ "<<=", TT_SHIFT_LEFT_ASSIGN },
        TokenValue{ "+=", TT_PLUS_ASSIGN },
        TokenValue{ "-=", TT_MINUS_ASSIGN },
        TokenValue{ "*=", TT_STAR_ASSIGN },
        TokenValue{ "/=", TT_SLASH_ASSIGN },
        TokenValue{ "%=", TT_MODULO_ASSIGN },
        TokenValue{ "&=", TT_AMP_ASSIGN },
        TokenValue{ "^=", TT_CARET_ASSIGN },
        TokenValue{ "|=", TT_PIPE_ASSIGN  },
        TokenValue{ ">>", TT_SHIFT_RIGHT },
        TokenValue{ "<<", TT_SHIFT_LEFT },
        TokenValue{ "++", TT_PLUS_PLUS },
        TokenValue{ "--", TT_MINUS_MINUS },
        TokenValue{ "->", TT_ARROW },
        TokenValue{ "&&", TT_AMP_AMP },
        TokenValue{ "||", TT_PIPE_PIPE },
        TokenValue{ "<=", TT_LE },
        TokenValue{ ">=", TT_GE },
        TokenValue{ "==", TT_ASSIGN },
        TokenValue{ "!=", TT_NOT_EQUALS },
        TokenValue{ ";", TT_SEMICOLON },
        TokenValue{ "{", TT_LBRACE },
        TokenValue{ "}", TT_RBRACE },
        TokenValue{ ",", TT_COMMA },
        TokenValue{ ":", TT_COLON },
        TokenValue{ "=", TT_EQUALS },
        TokenValue{ "(", TT_LPAREN },
        TokenValue{ ")", TT_RPAREN },
        TokenValue{ "[", TT_LBRACKET },
        TokenValue{ "]", TT_RBRACKET },
        TokenValue{ ".", TT_DOT },
        TokenValue{ "&", TT_AMP },
        TokenValue{ "!", TT_NOT },
        TokenValue{ "~", TT_TILDE },
        TokenValue{ "-", TT_MINUS },
        TokenValue{ "+", TT_PLUS },
        TokenValue{ "*", TT_STAR },
        TokenValue{ "/", TT_SLASH },
        TokenValue{ "%", TT_MODULO },
        TokenValue{ "<", TT_LT },
        TokenValue{ ">", TT_GT },
        TokenValue{ "^", TT_CARET },
        TokenValue{ "|", TT_PIPE },
        TokenValue{ "?", TT_QMARK },
    };
    for(const auto& token: tokens) {
        VerifyScanSingleToken(token.first, token.second);
    }
}
