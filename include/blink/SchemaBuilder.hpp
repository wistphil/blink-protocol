#pragma once

#include "blink/Schema.hpp"
#include "blink/Token.hpp"
#include "blink/Util.hpp"

#include <span>

namespace blink {

class SchemaBuilder
{
    using TokenIterator = std::span<Token>::iterator;

public:
    SchemaBuilder(std::span<Token> tokens);
    auto get_messages() -> std::vector<Message>;

private:
    auto get_message() -> Result<Message>;
    auto get_fields() -> Result<std::vector<Field>>;
    auto get_field() -> Result<Field>;

private:
    TokenIterator iterator_;
};

} // namespace blink {
