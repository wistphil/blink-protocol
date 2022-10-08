#include "blink/Lexer.hpp"

#include "fmt/format.h"

#include <cctype>
#include <format>

namespace blink {

Lexer::Lexer(CharReader reader)
    : reader_(std::move(reader))
    , keywords_(create_keywords())
{
}

auto Lexer::create_keywords() -> std::unordered_set<std::string>
{
    return std::unordered_set<std::string>{
            "i8", "u8", "i16", "u16", "i32", "u32",
            "i64", "u64","f64", "decimal", "date",
            "timeOfDayMilli", "timeOfDayNano",
            "nanotime", "millitime", "bool", "string",
            "binary", "fixed", "object", "namespace",
            "type", "schema"};
}

void Lexer::increment()
{
    reader_.increment();
    ++current_column_;
    if (reader_.is_end()) {
        return;
    }
    if (reader_.current() == '\n') {
        ++current_line_;
        current_column_ = 1;
    }
}

auto Lexer::increment(int count) -> void
{
    for (int i{0}; i < count; ++i) {
        increment();
    }
}

auto Lexer::tokenize() -> std::vector<Token>
{
    std::vector<Token> result;
    for (bool stop{false}; !stop; ) {
        result.push_back(get_next_token());
        if (result.back().type == TokenClass::EndOfFile ||
                result.back().type == TokenClass::Error) {
            stop = true;
        }
    }
    return result;
}

auto Lexer::get_next_token() -> Token
{
    while (!reader_.is_end() && is_whitespace(reader_.current())) {
        increment();
    }

    if (reader_.is_end()) {
        return get_end_of_file();
    }

    switch (reader_.current()) {
        case ',': {
            return get_expected_token(TokenClass::Comma, ",");
        }
        case '(': {
            return get_expected_token(TokenClass::OpenParen, "(");
        }
        case ')': {
            return get_expected_token(TokenClass::CloseParen, ")");
        }
        case '[': {
            return get_expected_token(TokenClass::OpenRectBracket, "[");
        }
        case ']': {
            return get_expected_token(TokenClass::CloseRectBracket, "]");
        } 
        case '?': {
            return get_expected_token(TokenClass::Opt, "?");
        }
        case '/': {
            return get_expected_token(TokenClass::Slash, "/");
        }
        case '-': {
            return get_expected_token(TokenClass::Body, "->");
        }
    }
    
    if (is_name_start_char(reader_.current())) {
        return get_identifier();
    }
    else if (is_digit(reader_.current())) {
        return get_uint();
    }

    auto error_char = reader_.current();
    increment();
    return get_error(fmt::format("Unable to match token starting with '{}'", error_char));
}

auto Lexer::get_end_of_file() -> Token
{
    return Token{
            .representation = "End Of File",
            .type = TokenClass::EndOfFile,
            .line = current_line_,
            .column = current_column_};
}

auto Lexer::get_error(std::string reason) -> Token
{
    return Token{
            .representation = std::move(reason),
            .type = TokenClass::Error,
            .line = current_line_,
            .column = current_column_};
}

auto Lexer::get_expected_token(TokenClass token_class, std::string expected_rep) -> Token
{
    const int line = current_line_;
    const int start_col = current_column_;

    for (auto ch : expected_rep) {
        if (reader_.is_end()) {
            return get_error("Expected '>'. Found End of File");
        }
        else if (reader_.current() != ch) {
            return get_error(fmt::format("Expected '>'. Found '{}'", reader_.current()));
        }
        increment();
    }

    return Token{std::move(expected_rep), token_class, line, start_col};
}

auto Lexer::get_identifier() -> Token
{
    const int line = current_line_;
    const int start_col = current_column_;
    std::string representation;

    while (!reader_.is_end() &&
            (is_name_start_char(reader_.current()) || is_digit(reader_.current()))) {
        representation.push_back(reader_.current());
        increment();
    }

    const TokenClass token_class = (keywords_.find(representation) == keywords_.end())
            ? TokenClass::Name : TokenClass::Keyword;

    return Token{std::move(representation), token_class, line, start_col};
}

auto Lexer::get_uint() -> Token
{
    const int line = current_line_;
    const int start_col = current_column_;
    std::string representation;

    while (!reader_.is_end() && is_digit(reader_.current())) {
        representation.push_back(reader_.current());
        increment();
    }

    return Token{std::move(representation), TokenClass::UInt, line, start_col};
}

auto Lexer::is_whitespace(char ch) -> bool
{
    return std::isspace(ch) != 0;
}

auto Lexer::is_digit(char ch) -> bool
{
    return std::isdigit(ch) != 0;
}

auto Lexer::is_name_start_char(char ch) -> bool
{
    return ch == '_' || (std::isalpha(ch) != 0);
}

} // namespace blink {
