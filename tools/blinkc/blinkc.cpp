#include "blink/Lexer.hpp"
#include "blink/SchemaBuilder.hpp"

#include "fmt/format.h"
#include "inja/inja.hpp"

#include <filesystem>
#include <fstream>
#include <vector>

const std::string template_str(R"(#pragma once

#include "blink/DynamicGroupImpl.hpp"
{% for message in messages %}
class {{message.name}}
{
public:
    explicit {{message.name}}(std::span<std::uint8_t> data)
        : impl_({{message.id}}, {{message.data_area_offset}}, data)
    { }

    constexpr auto get_name() const -> std::string_view { return "{{message.name}}"; }
    auto get_type_id() const -> std::uint64_t { return impl_.get_type_id(); }
    auto size() const -> std::size_t { return impl_.size(); }
    {% for field in message.fields %}
    auto set_{{field.name}}({{field.type}} {{field.name}}) -> void { impl_.set_field({{field.storage}}, {{field.offset}}, {{field.name}}{% if existsIn(field, "max_length") %}, {{field.max_length}}u{% endif %}); }
    auto get_{{field.name}}() const -> {{field.type}} { return impl_.get_field<{{field.type}}>({{field.storage}}, {{field.offset}}); }
    {% endfor %}
private:
    blink::DynamicGroupImpl impl_;
};
{% endfor %}
)");

auto load_file(std::filesystem::path path) -> std::vector<char>
{
    std::ifstream fin(path);
    if (!fin) {
        return {};
    }
    return std::vector<char>(
            std::istreambuf_iterator<char>{fin},
            std::istreambuf_iterator<char>{});
}

auto process_schema(std::span<blink::Message> messages) -> inja::json
{
    inja::json data;
    for (const auto & message : messages) {
        inja::json msg;
        msg["name"] = message.name;
        msg["id"] = message.id.value_or(0);
        int offset{0};
        for (const auto & field : message.fields) {
            inja::json json_field;
            json_field["offset"] = offset;
            json_field["name"] = field.name;
            if (field.id) {
                json_field["id"] = *field.id;
            }
            auto type = blink::field_type::to_cpp_type(field.type);
            if (field.is_optional) {
                type = fmt::format("std::optional<{}>", type);
            }
            json_field["type"] = type;
            if (field.max_length) {
                json_field["max_length"] = *field.max_length;
            }
            if (blink::field::is_inline(field)) {
                json_field["storage"] = "blink::InlineStorage{}";
            }
            else {
                json_field["storage"] = "blink::IndirectStorage{}";
            }
            offset += blink::field::calculate_inline_size(field);
            msg["fields"].push_back(json_field);
        }
        msg["data_area_offset"] = offset;
        data["messages"].push_back(msg);
    }

    return data;
}

auto main(int argc, char * argv[]) -> int
{
    if (argc != 3) {
        return -1;
    }

    std::filesystem::path schema_file(argv[1]);
    std::filesystem::path output_file(argv[2]);

    auto schema_contents = load_file(schema_file);
    if (schema_contents.empty()) {
        return -1;
    }

    auto tokens = blink::Lexer{blink::CharReader(schema_contents)}.tokenize();
    blink::SchemaBuilder schema_builder{tokens};
    auto messages = schema_builder.get_messages();

    if (messages.empty()) {
        return -1;
    }

    auto data = process_schema(messages);

    std::ofstream fout(output_file);
    if (!fout.is_open()) {
        return -1;
    }

    inja::render_to(fout, template_str, data);

    return 0;
}
