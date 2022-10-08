#include "blink/SchemaBuilder.hpp"

namespace blink {

SchemaBuilder::SchemaBuilder(std::span<Token> tokens)
    : iterator_(tokens.begin())
{
}

auto SchemaBuilder::get_messages() -> std::vector<Message>
{
    std::vector<Message> messages;
    while (iterator_->type != TokenClass::EndOfFile) {
        auto res = get_message();
        if (holds_error(res)) {
            return {};
        }
        messages.push_back(get_value(std::move(res)));
    }
    return messages;
}

auto SchemaBuilder::get_message() -> Result<Message>
{
    if (iterator_->type != TokenClass::Name) {
        return std::string{};
    }

    Message msg;
    msg.name = iterator_->representation;
    ++iterator_;
    if (iterator_->type == TokenClass::Slash) {
        ++iterator_;
        if (iterator_->type != TokenClass::UInt) {
            return std::string{};
        }
        msg.id = from_string<std::uint64_t>(iterator_->representation);
        if (!msg.id) {
            return std::string{};
        }
        ++iterator_;
    }

    if (iterator_->type != TokenClass::Body) {
        return std::string{};
    }

    ++iterator_;
    auto fields = get_fields();
    if (holds_error(fields)) {
        return std::string{};
    }
    msg.fields = get_value(std::move(fields));

    return msg;
}

auto SchemaBuilder::get_fields() -> Result<std::vector<Field>>
{
    std::vector<Field> fields;

    auto first_field = get_field();
    if (holds_error(first_field)) {
        return get_error_message(first_field);
    }
    fields.push_back(get_value(std::move(first_field)));

    while (iterator_->type == TokenClass::Comma) {
        ++iterator_;
        auto field = get_field();
        if (holds_error(field)) {
            return get_error_message(field); 
        }
        fields.push_back(get_value(std::move(field)));
    }
    return fields;
}

auto SchemaBuilder::get_field() -> Result<Field>
{
    if (iterator_->type != TokenClass::Keyword) {
        return std::string{};
    }
    Field field;
    auto field_type = field_type::from_string(iterator_->representation);
    if (!field_type) {
        return std::string{};
    }
    field.type = *field_type;
    ++iterator_;
    if (iterator_->type == TokenClass::OpenParen) {
        ++iterator_;
        if (iterator_->type != TokenClass::UInt) {
            return std::string{};
        }
        field.max_length = from_string<std::uint8_t>(iterator_->representation);
        if (!field.max_length) {
            return std::string{};
        }
        ++iterator_;
        if (iterator_->type != TokenClass::CloseParen) {
            return std::string{};
        }
        ++iterator_;
    }
    if (iterator_->type != TokenClass::Name) {
        return std::string{};
    }
    field.name = iterator_->representation;
    ++iterator_;
    if (iterator_->type == TokenClass::Slash) {
        ++iterator_;
        if (iterator_->type != TokenClass::UInt) {
            return std::string{};
        }
        field.id = from_string<std::uint64_t>(iterator_->representation);
        if (!field.id) {
            return std::string{};
        }
        ++iterator_;
    }
    if (iterator_->type == TokenClass::Opt) {
        field.is_optional = true;
        ++iterator_;
    }

    return field;
}

} // namespace blink {
