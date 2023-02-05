#pragma once

#include <cstdint>
#include <optional>
#include <ostream>
#include <string>
#include <variant>
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

struct TypeInfo
{
    std::string representation;
    std::optional<FieldType> type;

    auto operator==(const TypeInfo & lhs) const -> bool = default;
};


namespace field_type {

auto to_string(FieldType type) -> std::string;
auto to_cpp_type(FieldType type) -> std::string;
auto from_string(const std::string & str) -> std::optional<FieldType>;

} // namespace field_type {

struct Field
{
    std::string name;
    std::optional<std::uint64_t> id;
    TypeInfo type_info;
    std::size_t inline_size{0};
    std::size_t indirect_size{0};
    std::optional<std::uint8_t> max_length;
    bool is_optional{false};
    bool is_sequence{false};

    auto operator==(const Field & field) const -> bool = default;
};

std::ostream & operator<<(std::ostream & os, const Field & field);

namespace field {

auto is_inline(const Field & field) -> bool;
auto calculate_inline_size(FieldType type, std::optional<std::size_t> max_length, bool is_optional, bool is_sequence) -> std::size_t;
auto get_signature(const Field & field) -> std::string;
auto to_cpp_type(const Field & field) -> std::string;

} // namespace field {

#if 0
struct Group
{
    std::string name;
    std::vector<std::variant<Group, Field>> fields;

    auto operator==(const Group & group) const -> bool = default;
};

struct DynamicGroup
{
    std::string name;
    std::optional<std::uint64_t> id;
    std::vector<std::variant<DynamicGroup, Group, Field>> fields;

    auto operator==(const DynamicGroup & dynamic_group) const -> bool = default;
};
#endif

struct Message
{
    std::string name;
    std::optional<std::uint64_t> id;
    std::size_t size{0};
    std::vector<Field> fields;

    auto operator==(const Message & message) const -> bool = default;
};

std::ostream & operator<<(std::ostream & os, const Message & msg);

class Schema
{
};

} // namespace blink {
