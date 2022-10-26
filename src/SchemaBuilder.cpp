#include "blink/SchemaBuilder.hpp"

namespace blink {

SchemaBuilder::SchemaBuilder(std::span<Token> tokens)
    : iterator_(tokens.begin())
{
    build();
}

auto SchemaBuilder::get_namespace() const -> const std::string &
{
    return namespace_;
}

auto SchemaBuilder::get_messages() const -> const std::vector<Message> &
{
    return messages_;
}

auto SchemaBuilder::build() -> void
{
    namespace_ = do_get_namespace();
    if (!do_get_messages()) {
        messages_.clear();
    }
}

auto SchemaBuilder::do_get_namespace() -> std::string
{
    std::string result;
    if (iterator_->type == TokenClass::Keyword &&
            iterator_->representation == "namespace") {
        ++iterator_;
        if (iterator_->type == TokenClass::Name) {
            result = iterator_->representation;
            ++iterator_;
        }
    }
    return result;
}

auto SchemaBuilder::do_get_messages() -> bool
{
    while (iterator_->type != TokenClass::EndOfFile) {
        auto res = get_message();
        if (holds_error(res)) {
            return false;
        }
        messages_.push_back(get_value(std::move(res)));
        message_names_.emplace(messages_.back().name, &messages_.back());
    }
    return true;
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
    auto res = get_fields();
    if (holds_error(res)) {
        return std::string{};
    }
    const auto & [size, fields] = get_value(res);
    msg.size = size;
    msg.fields = fields;
    return msg;
}

auto SchemaBuilder::get_fields() -> Result<std::pair<std::size_t, std::vector<Field>>>
{
    std::vector<Field> fields;
    std::size_t size{0};

    auto first_field = get_field();
    if (holds_error(first_field)) {
        return get_error_message(first_field);
    }
    fields.push_back(get_value(std::move(first_field)));
    size += fields.back().size;

    while (iterator_->type == TokenClass::Comma) {
        ++iterator_;
        auto field = get_field();
        if (holds_error(field)) {
            return get_error_message(field); 
        }
        fields.push_back(get_value(std::move(field)));
        size += fields.back().size;
    }
    return std::make_pair(size, std::move(fields));
}

auto SchemaBuilder::get_field() -> Result<Field>
{
    auto type_info = get_type_info();
    if (!type_info) {
        return std::string{};
    }
    std::optional<std::uint8_t> max_length;
    if (iterator_->type == TokenClass::OpenParen) {
        ++iterator_;
        if (iterator_->type != TokenClass::UInt) {
            return std::string{};
        }
        max_length = from_string<std::uint8_t>(iterator_->representation);
        if (!max_length) {
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
    auto name = iterator_->representation;
    ++iterator_;
    std::optional<std::uint64_t> id;
    if (iterator_->type == TokenClass::Slash) {
        ++iterator_;
        if (iterator_->type != TokenClass::UInt) {
            return std::string{};
        }
        id = from_string<std::uint64_t>(iterator_->representation);
        if (!id) {
            return std::string{};
        }
        ++iterator_;
    }
    bool is_optional{false};
    if (iterator_->type == TokenClass::Opt) {
        is_optional = true;
        ++iterator_;
    }

    std::size_t size{0};
    if (!type_info->type) {
        auto it = message_names_.find(type_info->representation);
        if (it == message_names_.end()) {
            return std::string{};
        }
        size = it->second->size;
    }
    else {
        size = field::calculate_inline_size(*type_info->type, max_length, is_optional);
    }

    return Field{
            .name = name,
            .id = id,
            .type_info = *type_info,
            .size = size,
            .max_length = max_length,
            .is_optional = is_optional};
}

auto SchemaBuilder::get_type_info() -> std::optional<TypeInfo>
{
    if (iterator_->type == TokenClass::Keyword) {
        auto field_type = field_type::from_string(iterator_->representation);
        if (!field_type) {
            return {};
        }
        ++iterator_;
        TypeInfo type_info;
        type_info.representation = field_type::to_string(*field_type);
        type_info.type = *field_type;
        return type_info;
    }

    if (iterator_->type != TokenClass::Name) {
        return {};
    }

    auto type_name = iterator_->representation;
    ++iterator_;
    return TypeInfo{.representation = type_name};
}

} // namespace blink {
