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
    explicit SchemaBuilder(std::span<Token> tokens);
    auto get_namespace() const -> const std::string &;
    auto get_messages() const -> const std::vector<Message> &;

private:
    auto build() -> void;
    auto do_get_namespace() -> std::string;
    auto do_get_messages() -> std::vector<Message>;
    auto get_message() -> Result<Message>;
    auto get_fields() -> Result<std::vector<Field>>;
    auto get_field() -> Result<Field>;

private:
    TokenIterator iterator_;
    std::string namespace_;
    std::vector<Message> messages_;
};

} // namespace blink {
