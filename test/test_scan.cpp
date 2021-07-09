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
    EXPECT_THROW(VerifyScanSingleToken("~", static_cast<TOKEN_TYPE>(0)), FatalError);
    EXPECT_EQ("unrecognized character '~' on line 1", fh.fatal_msg);
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

TEST(Scan, Token_Int) {
    VerifyScanToken("int", K_INT);
}
