#pragma once

#include <cstdint>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace blink {

enum class FieldType
{
    I8,
    U8,
    I16,
    U16,
    I32,
    U32,
    I64,
    U64,
    F64,
    Decimal,
    Date,
    TimeOfDayMilli,
    TimeOfDayNano,
    NanoTime,
    MilliTime,
    Bool,
    String,
    Binary,
    Fixed
};

namespace field_type {

std::string to_string(FieldType type);
std::string to_cpp_type(FieldType type);
std::optional<FieldType> from_string(const std::string & str);

} // namespace field_type {

struct Field
{
    std::string name;
    std::optional<std::uint64_t> id;
    FieldType type;
    std::optional<std::uint8_t> max_length;
    bool is_optional{false};

    auto operator==(const Field & field) const -> bool = default;
};

std::ostream & operator<<(std::ostream & os, const Field & field);

namespace field {

auto is_inline(const Field & field) -> bool;
auto calculate_inline_size(const Field & field) -> std::size_t;

} // namespace field {

struct Message
{
    std::string name;
    std::optional<std::uint64_t> id;
    std::vector<Field> fields;

    auto operator==(const Message & message) const -> bool = default;
};

std::ostream & operator<<(std::ostream & os, const Message & msg);

class Schema
{
};

} // namespace blink {
