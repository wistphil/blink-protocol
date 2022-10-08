#pragma once

#include "blink/CharReader.hpp"
#include "blink/Token.hpp"

#include <string>
#include <unordered_set>
#include <vector>

namespace blink {

class Lexer
{
public:
    explicit Lexer(CharReader reader);

    auto tokenize() -> std::vector<Token>;

private:
    static auto create_keywords() -> std::unordered_set<std::string>;
    auto increment() -> void;
    auto increment(int count) -> void;
    auto get_next_token() -> Token;
    auto get_end_of_file() -> Token;
    auto get_error(std::string reason) -> Token;
    auto get_expected_token(TokenClass token_class, std::string expected_rep) -> Token;
    auto get_identifier() -> Token;
    auto get_uint() -> Token;

    static auto is_whitespace(char ch) -> bool;
    static auto is_digit(char ch) -> bool;
    static auto is_name_start_char(char ch) -> bool;

private:
    CharReader reader_;
    std::unordered_set<std::string> keywords_;
    int current_line_{1};
    int current_column_{1};
};

} // namespace blink {
