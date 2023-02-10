#include "blink/Lexer.hpp"
#include "blink/SchemaBuilder.hpp"

#include "fmt/format.h"
#include "inja/inja.hpp"

#include <filesystem>
#include <fstream>
#include <vector>

const std::string HeaderTemplateStr(R"(#pragma once

#include "blink/DynamicGroupImpl.hpp"
{% if include_sequence_header %}
#include "blink/Sequence.hpp"
{% endif %}

#include <iosfwd>

{% if exists("namespace") %}
namespace {{namespace}} {
{% endif %}

{% for message in messages %}
class {{message.name}}
{
public:
    {% if message.is_group %}
    explicit {{message.name}}(blink::GroupImpl group)
        : impl_(group)
    { }

    {% else %}
    explicit {{message.name}}(std::span<std::uint8_t> data)
        : impl_({{message.id}}, {{message.data_area_offset}}, data)
    { }

    static constexpr std::uint64_t type_id{ {{message.id}} };

    auto get_preamble() const -> blink::DynamicGroupPreamble { return impl_.get_preamble(); }
    auto set_preamble_size() -> void { impl_.set_preamble_size(); }
    {% endif %}
    constexpr auto get_name() const -> std::string_view { return "{{message.name}}"; }
    auto size() const -> std::size_t { return impl_.size(); }
    auto data() const -> const std::uint8_t * { return impl_.data(); }

    {% for field in message.fields %}
    {% if field.is_group %}
    {% if field.is_sequence %}
    auto init_{{field.name}}(std::size_t count) -> {{field.cpp_type}} { return {{field.cpp_type}}(impl_.init_sequence({{field.offset}}, {{field.indirect_size}}, count)); }
    auto get_{{field.name}}() const -> {{field.cpp_type}} { return {{field.cpp_type}}(impl_.get_sequence({{field.offset}}, {{field.indirect_size}})); }
    {% else %}
    auto get_{{field.name}}() const -> {{field.cpp_type}} { return {{field.cpp_type}}(impl_.get_group({{field.offset}}, {{field.inline_size}})); }
    {% endif %}
    {% else %}
    {% if existsIn(field, "max_length") %}
    auto set_{{field.name}}({{field.cpp_type}} {{field.name}}) -> void { impl_.set_{{field.field_type}}({{field.offset}}, {{field.name}}, {{field.max_length}}u); }
    {% else %}
    auto set_{{field.name}}({{field.cpp_type}} {{field.name}}) -> void { impl_.set_{{field.field_type}}({{field.offset}}, {{field.name}}); }
    {% endif %}
    auto get_{{field.name}}() const -> {{field.cpp_type}} { return impl_.get_{{field.field_type}}<{{field.cpp_type}}>({{field.offset}}); }
    {% endif %}
    {% endfor %}

private:
    {% if message.is_group %}
    blink::GroupImpl impl_;
    {% else %}
    blink::DynamicGroupImpl impl_;
    {% endif %}
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
    blink::Indenter indenter(os);
    {% if message.is_group %}
    os << "->";
    {% else %}
    os << "{{message.name}}/{{message.id}} (" << msg.get_preamble().get_size() << ") ->";
    {% endif %}
    {% for field in message.fields %}
    {% if field.is_group %}
    {% if field.is_sequence %}
    {
        blink::Indenter seq_indenter(os);
        auto {{field.name}} = msg.get_{{field.name}}();
        os << '\n' << indenter.indent() << "{{field.signature}} (" << {{field.name}}.size() << ") ->";
        for (std::size_t i{0}; i < {{field.name}}.size(); ++i) {
            os << '\n' << seq_indenter.indent() << "{{field.name}} [" << i << "] " << blink::make_printer({{field.name}}[i]);
        }
    }
    {% else %}
    os << '\n' << indenter.indent() << "{{field.signature}} " << blink::make_printer(msg.get_{{field.name}}());
    {% endif %}
    {% else %}
    os << '\n' << indenter.indent() << "{{field.signature}} = " << blink::make_printer(msg.get_{{field.name}}());
    {% endif %}
    {% endfor %}
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
    bool include_sequence_header{false};
    for (const auto & message : messages) {
        inja::json msg;
        msg["name"] = message.name;
        int offset{0};
        for (const auto & field : message.fields) {
            inja::json json_field;
            json_field["offset"] = offset;
            json_field["inline_size"] = field.inline_size;
            json_field["indirect_size"] = field.indirect_size;
            json_field["name"] = field.name;
            json_field["cpp_type"] = blink::field::to_cpp_type(field);
            json_field["is_group"] = !field.type_info.type.has_value();
            json_field["is_sequence"] = field.is_sequence;
            if (field.is_sequence) {
                include_sequence_header = true;
            }
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
            offset += field.inline_size;
            msg["fields"].push_back(json_field);
        }
        if (message.id) {
            msg["is_group"] = false;
            msg["id"] = *message.id;
            msg["data_area_offset"] = offset;
        }
        else {
            msg["is_group"] = true;
        }
        data["messages"].push_back(msg);
        data["include_sequence_header"] = include_sequence_header;
    }

    return data;
}

bool render_to_file(const std::filesystem::path & output_file, std::string_view str, const inja::json & data)
{
    std::ofstream fout(output_file);
    if (!fout.is_open()) {
        return false;
    }

    inja::Environment env;
    env.set_trim_blocks(true);
    env.set_lstrip_blocks(true);
    env.render_to(fout, env.parse(str), data);

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
