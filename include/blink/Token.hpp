#pragma once

#include <string>

namespace blink {

enum class TokenClass
{
    Slash, // "/"
    Comma, // ","
    OpenParen, // "("
    CloseParen, // ")"
    OpenRectBracket, // "["
    CloseRectBracket, // "]"
    Opt, // "?"
    Body, // "->"
    Name, 
    Keyword,
    UInt,
    Error,
    EndOfFile
};

namespace token_class {

std::string to_string(TokenClass token_class);

} // namespace token_class {


struct Token
{
    std::string representation;
    TokenClass type;
    int line;
    int column;
};

} // namespace blink {
