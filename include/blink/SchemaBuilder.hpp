#pragma once

#include "blink/Schema.hpp"
#include "blink/Token.hpp"
#include "blink/Util.hpp"

#include <optional>
#include <span>
#include <unordered_map>

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
    auto do_get_messages() -> bool;
    auto get_message() -> Result<Message>;
    auto get_fields() -> Result<std::pair<std::size_t, std::vector<Field>>>;
    auto get_field() -> Result<Field>;
    auto get_type_info() -> std::optional<TypeInfo>;

private:
    TokenIterator iterator_;
    std::string namespace_;
    std::vector<Message> messages_;
    std::unordered_map<std::string, Message *> message_names_;
};

} // namespace blink {
