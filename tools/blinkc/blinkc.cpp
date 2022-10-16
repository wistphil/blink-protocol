#include "blink/Lexer.hpp"
#include "blink/SchemaBuilder.hpp"

#include "fmt/format.h"
#include "inja/inja.hpp"

#include <filesystem>
#include <fstream>
#include <vector>

const std::string HeaderTemplateStr(R"(#pragma once

#include "blink/DynamicGroupImpl.hpp"

#include <iosfwd>
{% if exists("namespace") %}
namespace {{namespace}} {
{% endif %}
{% for message in messages %}
class {{message.name}}
{
public:
    explicit {{message.name}}(std::span<std::uint8_t> data)
        : impl_({{message.id}}, {{message.data_area_offset}}, data)
    { }

    constexpr auto get_name() const -> std::string_view { return "{{message.name}}"; }
    auto get_preamble() const -> blink::DynamicGroupPreamble { return impl_.get_preamble(); }
    auto size() const -> std::size_t { return impl_.size(); }
    auto data() const -> const std::uint8_t * { return impl_.data(); }
    {% for field in message.fields %}
    auto set_{{field.name}}({{field.cpp_type}} {{field.name}}) -> void { impl_.set_{{field.field_type}}({{field.offset}}, {{field.name}}{% if existsIn(field, "max_length") %}, {{field.max_length}}u{% endif %}); }
    auto get_{{field.name}}() const -> {{field.cpp_type}} { return impl_.get_{{field.field_type}}<{{field.cpp_type}}>({{field.offset}}); }
    {% endfor %}
private:
    blink::DynamicGroupImpl impl_;
};

std::ostream & operator<<(std::ostream & os, const {{message.name}} & msg);
{% endfor %}
{% if exists("namespace") %}
} // namespace {{namespace}} {
{% endif %}
)");

const std::string CppTemplateStr(R"(#include "{{file_stem}}.hpp"

#include "blink/PrintHelpers.hpp"

#include <ostream>
{% if exists("namespace") %}
namespace {{namespace}} {
{% endif %}
{% for message in messages %}
std::ostream & operator<<(std::ostream & os, const {{message.name}} & msg)
{
    os << "{{message.name}}/{{message.id}} (" << msg.get_preamble().get_size() << ") ->";
    {% for field in message.fields %}
    os << "\n    {{field.signature}} = " << blink::make_printer(msg.get_{{field.name}}());
    {% endfor %}
    os << '\n';
    return os;
}
{% endfor %}
{% if exists("namespace") %}
} // namespace {{namespace}} {
{% endif %}
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

auto process_schema(const blink::SchemaBuilder & builder, const std::filesystem::path & stem) -> inja::json
{
    inja::json data;

    if (auto ns = builder.get_namespace(); !ns.empty()) {
        data["namespace"] = ns;
    }

    auto messages = builder.get_messages();

    if (messages.empty()) {
        return data;
    }

    data["file_stem"] = stem.c_str();
    for (const auto & message : messages) {
        inja::json msg;
        msg["name"] = message.name;
        msg["id"] = message.id.value_or(0);
        int offset{0};
        for (const auto & field : message.fields) {
            inja::json json_field;
            json_field["offset"] = offset;
            json_field["name"] = field.name;
            json_field["cpp_type"] = blink::field::to_cpp_type(field);
            if (field.max_length) {
                json_field["max_length"] = *field.max_length;
            }
            if (blink::field::is_inline(field)) {
                json_field["field_type"] = "inline_field";
            }
            else {
                json_field["field_type"] = "indirect_field";
            }
            json_field["signature"] = blink::field::get_signature(field);
            offset += blink::field::calculate_inline_size(field);
            msg["fields"].push_back(json_field);
        }
        msg["data_area_offset"] = offset;
        data["messages"].push_back(msg);
    }

    return data;
}

bool render_to_file(const std::filesystem::path & output_file, std::string_view str, const inja::json & data)
{
    std::ofstream fout(output_file);
    if (!fout.is_open()) {
        return false;
    }

    inja::render_to(fout, str, data);

    return true;
}

auto main(int argc, char * argv[]) -> int
{
    if (argc != 3) {
        fmt::print("usage: blinkc <shema-file> <output-path>\n");
        return -1;
    }

    const std::filesystem::path schema_file(argv[1]);
    std::filesystem::path output_path(argv[2]);

    auto schema_contents = load_file(schema_file);
    if (schema_contents.empty()) {
        return -1;
    }

    auto tokens = blink::Lexer{blink::CharReader(schema_contents)}.tokenize();
    blink::SchemaBuilder schema_builder{tokens};

    const auto stem = schema_file.stem();
    auto data = process_schema(schema_builder, stem);

    const auto header_filename = fmt::format("{}.hpp", stem.c_str());
    const auto cpp_filename = fmt::format("{}.cpp", stem.c_str());

    if (!render_to_file(output_path.replace_filename(header_filename), HeaderTemplateStr, data)) {
        return -1;
    }

    if (!render_to_file(output_path.replace_filename(cpp_filename), CppTemplateStr, data)) {
        return -1;
    }

    return 0;
}
