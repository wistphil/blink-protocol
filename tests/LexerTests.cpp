#include <gtest/gtest.h>

#include "blink/Lexer.hpp"

namespace blink::tests {

auto get_tokens(std::string_view str) -> std::vector<Token>
{
    return Lexer{CharReader(std::span<const char>(&str[0], str.size()))}.tokenize();
}

TEST(LexerTests, get_end_of_file)
{
    for (auto str : {"", "   ", "  \n\n  ", "\t  \n  "}) {
        auto tokens = get_tokens(str);
        ASSERT_EQ(1, tokens.size());
        EXPECT_EQ(TokenClass::EndOfFile, tokens[0].type);
    }
}

TEST(LexerTests, get_names)
{
    std::string str{"ABC  _123 \n def456   G"};
    auto tokens = get_tokens(str);

    ASSERT_EQ(5, tokens.size());
    for (const auto & token : std::span<Token>(tokens.begin(), tokens.size() - 1)) {
        EXPECT_EQ(TokenClass::Name, token.type);
    }
    EXPECT_EQ(TokenClass::EndOfFile, tokens.back().type);
}

TEST(LexerTests, get_body)
{
    auto tokens = get_tokens("->");

    ASSERT_EQ(2, tokens.size());
    EXPECT_EQ(TokenClass::Body, tokens[0].type);
}

TEST(LexerTests, get_keywords)
{
    std::string str{"string i32 u16 bool"};
    auto tokens = get_tokens(str);

    ASSERT_EQ(5, tokens.size());
    for (const auto & token : std::span<Token>(tokens.begin(), tokens.size() - 1)) {
        EXPECT_EQ(TokenClass::Keyword, token.type);
    }
    EXPECT_EQ(TokenClass::EndOfFile, tokens.back().type);
}

TEST(LexerTests, get_fields)
{
    std::string str{"i32 field1, i64 field2"};
    auto tokens = get_tokens(str);

    ASSERT_EQ(6, tokens.size());
    EXPECT_EQ(TokenClass::Keyword, tokens[0].type);
    EXPECT_EQ(TokenClass::Name, tokens[1].type);
    EXPECT_EQ(TokenClass::Comma, tokens[2].type);
    EXPECT_EQ(TokenClass::Keyword, tokens[3].type);
    EXPECT_EQ(TokenClass::Name, tokens[4].type);
    EXPECT_EQ(TokenClass::EndOfFile, tokens.back().type);
}

TEST(LexerTests, get_opt_field)
{
    std::string str{"i32 field1?"};
    auto tokens = get_tokens(str);

    ASSERT_EQ(4, tokens.size());
    EXPECT_EQ(TokenClass::Keyword, tokens[0].type);
    EXPECT_EQ(TokenClass::Name, tokens[1].type);
    EXPECT_EQ(TokenClass::Opt, tokens[2].type);
    EXPECT_EQ(TokenClass::EndOfFile, tokens.back().type);
}

TEST(LexerTests, get_max_length_field)
{
    std::string str{"string (16) name"};
    auto tokens = get_tokens(str);

    ASSERT_EQ(6, tokens.size());
    EXPECT_EQ(TokenClass::Keyword, tokens[0].type);
    EXPECT_EQ(TokenClass::OpenParen, tokens[1].type);
    EXPECT_EQ(TokenClass::UInt, tokens[2].type);
    EXPECT_EQ(TokenClass::CloseParen, tokens[3].type);
    EXPECT_EQ(TokenClass::Name, tokens[4].type);
    EXPECT_EQ(TokenClass::EndOfFile, tokens.back().type);
}

TEST(LexerTests, get_name_with_id)
{
    std::string str{"Car/42"};
    auto tokens = get_tokens(str);

    ASSERT_EQ(4, tokens.size());
    EXPECT_EQ(TokenClass::Name, tokens[0].type);
    EXPECT_EQ(TokenClass::Slash, tokens[1].type);
    EXPECT_EQ(TokenClass::UInt, tokens[2].type);
    EXPECT_EQ(TokenClass::EndOfFile, tokens.back().type);
}

TEST(LexerTests, get_dyn_group)
{
    std::string str{"Car -> string make, string model, i32 year, i32 miles?"};
    auto tokens = get_tokens(str);

    ASSERT_EQ(15, tokens.size());
    EXPECT_EQ(TokenClass::Name, tokens[0].type);
    EXPECT_EQ(TokenClass::Body, tokens[1].type);
    EXPECT_EQ(TokenClass::Keyword, tokens[2].type);
    EXPECT_EQ(TokenClass::Name, tokens[3].type);
    EXPECT_EQ(TokenClass::Comma, tokens[4].type);
    EXPECT_EQ(TokenClass::Keyword, tokens[5].type);
    EXPECT_EQ(TokenClass::Name, tokens[6].type);
    EXPECT_EQ(TokenClass::Comma, tokens[7].type);
    EXPECT_EQ(TokenClass::Keyword, tokens[8].type);
    EXPECT_EQ(TokenClass::Name, tokens[9].type);
    EXPECT_EQ(TokenClass::Comma, tokens[10].type);
    EXPECT_EQ(TokenClass::Keyword, tokens[11].type);
    EXPECT_EQ(TokenClass::Name, tokens[12].type);
    EXPECT_EQ(TokenClass::Opt, tokens[13].type);
    EXPECT_EQ(TokenClass::EndOfFile, tokens.back().type);
}

} // namespace blink::tests
