#include "blink/Token.hpp"

namespace blink {

namespace token_class {

std::string to_string(TokenClass token_class)
{
    switch (token_class) {
        case TokenClass::Slash: return "Slash";
        case TokenClass::Comma: return "Comma";
        case TokenClass::OpenParen: return "OpenParen";
        case TokenClass::CloseParen: return "CloseParen";
        case TokenClass::OpenRectBracket: return "OpenRectBracket";
        case TokenClass::CloseRectBracket: return "CloseRectBracket";
        case TokenClass::Opt: return "Opt";
        case TokenClass::Body: return "Body";
        case TokenClass::Name: return "Name";
        case TokenClass::Keyword: return "Keyword";
        case TokenClass::UInt: return "UInt";
        case TokenClass::Error: return "Error";
        case TokenClass::EndOfFile: return "EndOfFile";
    }
    return "Unknown";
}

} // namespace token_class {

} // namespace blink {
