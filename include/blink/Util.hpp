#pragma once

#include <charconv>
#include <optional>
#include <string_view>
#include <variant>

namespace blink {

template <typename T>
using Result = std::variant<T, std::string>;

template <typename T>
auto holds_error(const Result<T> & res) -> bool
{
    return std::get_if<1>(&res) != nullptr;
}

template <typename T>
auto get_error_message(const Result<T> & res) -> const std::string &
{
    return std::get<1>(res);
}

template <typename T>
auto get_value(const Result<T> & res) -> const T &
{
    return std::get<0>(res);
}

template <typename T>
auto get_value(Result<T> && res) -> T &&
{
    return std::move(std::get<0>(res));
}

template <typename T>
auto from_string(std::string_view sv) -> std::optional<T>
{
    if (T value{}; std::from_chars(sv.begin(), sv.end(), value).ptr == sv.end()) {
        return value;
    }
    return {};
}

template <typename T>
struct Tag { using type = T; };

} // namespace blink {
