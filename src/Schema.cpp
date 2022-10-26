#include "blink/Schema.hpp"

#include "fmt/format.h"

#include <unordered_map>

namespace blink {

namespace {

const std::unordered_map<std::string, FieldType> str_to_field_type{
        {"i8", FieldType::I8},
        {"u8", FieldType::U8},
        {"i16", FieldType::I16},
        {"u16", FieldType::U16},
        {"i32", FieldType::I32},
        {"u32", FieldType::U32},
        {"i64", FieldType::I64},
        {"u64", FieldType::U64},
        {"f64", FieldType::F64},
        {"decimal", FieldType::Decimal},
        {"date", FieldType::Date},
        {"timeOfDayMilli", FieldType::TimeOfDayMilli},
        {"timeOfDayNano", FieldType::TimeOfDayNano},
        {"nanotime", FieldType::NanoTime},
        {"millitime", FieldType::MilliTime},
        {"bool", FieldType::Bool},
        {"string", FieldType::String},
        {"binary", FieldType::Binary},
        {"fixed", FieldType::Fixed}
};

} // anonymous namespace

namespace field_type {

auto to_string(FieldType type) -> std::string
{
    switch (type) {
        case FieldType::I8: return "i8";
        case FieldType::U8: return "u8";
        case FieldType::I16: return "i16";
        case FieldType::U16: return "u16";
        case FieldType::I32: return "i32";
        case FieldType::U32: return "u32";
        case FieldType::I64: return "i64";
        case FieldType::U64: return "u64";
        case FieldType::F64: return "f64";
        case FieldType::Decimal: return "decmimal";
        case FieldType::Date: return "date";
        case FieldType::TimeOfDayMilli: return "timeOfDayMilli";
        case FieldType::TimeOfDayNano: return "timeOfDayNano";
        case FieldType::NanoTime: return "nanotime";
        case FieldType::MilliTime: return "millitime";
        case FieldType::Bool: return "bool";
        case FieldType::String: return "string";
        case FieldType::Binary: return "binary";
        case FieldType::Fixed: return "fixed";
    }
}

auto to_cpp_type(FieldType type) -> std::string
{
    switch (type) {
        case FieldType::I8: return "std::int8_t";
        case FieldType::U8: return "std::uint8_t";
        case FieldType::I16: return "std::int16_t";
        case FieldType::U16: return "std::uint16_t";
        case FieldType::I32: return "std::int32_t";
        case FieldType::U32: return "std::uint32_t";
        case FieldType::I64: return "std::int64_t";
        case FieldType::U64: return "std::uint64_t";
        case FieldType::F64: return "double";
        case FieldType::Decimal: return "<unsupported>";
        case FieldType::Date: return "<unsupported>";
        case FieldType::TimeOfDayMilli: return "<unsupported>";
        case FieldType::TimeOfDayNano: return "<unsupported>";
        case FieldType::NanoTime: return "<unsupported>";
        case FieldType::MilliTime: return "<unsupported>";
        case FieldType::Bool: return "bool";
        case FieldType::String: return "std::string_view";
        case FieldType::Binary: return "std::span<std::uint8_t>";
        case FieldType::Fixed: return "<unsupported>";
    }
}

auto from_string(const std::string & str) -> std::optional<FieldType>
{
    auto it = str_to_field_type.find(str);
    if (it == str_to_field_type.end()) {
        return {};
    }

    return it->second;
}

} // namespace field_type {


std::ostream & operator<<(std::ostream & os, const Field & field)
{
    const std::string id = field.id ? std::to_string(*field.id) : std::string{"<empty>"};
    const std::string max_length = field.max_length ? std::to_string(*field.max_length) : std::string{"<empty>"};

    os << "Field: {name = " << field.name
            << ", id = " << id
            << ", type = " << field.type_info.representation
            << ", max_length = " << max_length
            << ", is_optional = " << std::boolalpha << field.is_optional << "}";

    return os;
}

namespace field {

auto is_inline(const Field & field) -> bool
{
    if (!field.type_info.type) {
        return true;
    }

    switch (*field.type_info.type) {
        case FieldType::I8: return true;
        case FieldType::U8: return true;
        case FieldType::I16: return true;
        case FieldType::U16: return true;
        case FieldType::I32: return true;
        case FieldType::U32: return true;
        case FieldType::I64: return true;
        case FieldType::U64: return true;
        case FieldType::F64: return true;
        case FieldType::Decimal: return true;
        case FieldType::Date: return true;
        case FieldType::TimeOfDayMilli: return true;
        case FieldType::TimeOfDayNano: return true;
        case FieldType::NanoTime: return true;
        case FieldType::MilliTime: return true;
        case FieldType::Bool: return true;
        case FieldType::String: {
            if (!field.max_length) {
                return false;
            }
            return true;
        }
        case FieldType::Binary: {
            if (!field.max_length) {
                return false;
            }
            return true;
        }
        case FieldType::Fixed: return true;
    }
}

auto calculate_inline_size(FieldType type, std::optional<std::size_t> max_length, bool is_optional) -> std::size_t
{
    const std::size_t additional_size = is_optional ? 1 : 0;

    switch (type) {
        case FieldType::I8: return sizeof(std::int8_t) + additional_size;
        case FieldType::U8: return sizeof(std::uint8_t) + additional_size;
        case FieldType::I16: return sizeof(std::int16_t) + additional_size;
        case FieldType::U16: return sizeof(std::uint16_t) + additional_size;
        case FieldType::I32: return sizeof(std::int32_t) + additional_size;
        case FieldType::U32: return sizeof(std::uint32_t) + additional_size;
        case FieldType::I64: return sizeof(std::int64_t) + additional_size;
        case FieldType::U64: return sizeof(std::uint64_t) + additional_size;
        case FieldType::F64: return sizeof(double) + additional_size;
        case FieldType::Decimal: return 0;
        case FieldType::Date: return 0;
        case FieldType::TimeOfDayMilli: return 0;
        case FieldType::TimeOfDayNano: return 0;
        case FieldType::NanoTime: return 0;
        case FieldType::MilliTime: return 0;
        case FieldType::Bool: return sizeof(bool) + additional_size;
        case FieldType::String: {
            if (!max_length) {
                return sizeof(std::uint32_t) + additional_size;
            }
            return *max_length + sizeof(std::uint8_t) + additional_size;
        }
        case FieldType::Binary: {
            if (!max_length) {
                return sizeof(std::uint32_t) + additional_size;
            }
            return *max_length + sizeof(std::uint8_t) + additional_size;
        }
        case  FieldType::Fixed: return max_length.value_or(0) + additional_size;
    }
}

auto get_signature(const Field & field) -> std::string
{
    std::string out;
    fmt::format_to(std::back_inserter(out), "{}", field.type_info.representation);
    if (field.max_length) {
        fmt::format_to(std::back_inserter(out), " ({})", *field.max_length);
    }
    fmt::format_to(std::back_inserter(out), " {}", field.name);
    if (field.id) {
        fmt::format_to(std::back_inserter(out), "/{}", *field.id);
    }
    if (field.is_optional) {
        fmt::format_to(std::back_inserter(out), "?");
    }
    return out;
}

auto to_cpp_type(const Field & field) -> std::string
{
    auto type = field.type_info.type ?
            field_type::to_cpp_type(*field.type_info.type) : field.type_info.representation;
    if (field.is_optional) {
        type = fmt::format("std::optional<{}>", type);
    }
    return type;
}

} // namespace field {

std::ostream & operator<<(std::ostream & os, const Message & msg)
{
    const std::string id = msg.id ? std::to_string(*msg.id) : std::string{"<empty>"};

    os << "Message: {name = " << msg.name << ", id = " << id << ", fields: [";
    for (const auto & field : msg.fields) {
        os << "\n    " << field;
    }
    os << "]\n}";
    return os;
}

} // namespace blink {
