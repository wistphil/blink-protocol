#pragma once

#include <cstdint>
#include <optional>
#include <ostream>
#include <type_traits>

namespace blink {

template <typename T>
struct Printer 
{
    T value;
};

template <typename T>
auto make_printer(T value)
{
    return Printer<T>{value};
}

template <typename T>
std::ostream & operator<<(std::ostream & os, Printer<T> printer)
{
    if constexpr (std::is_same_v<bool, T>) {
        os << std::boolalpha << printer.value;
    }
    else if constexpr (std::is_same_v<std::uint8_t, T>) {
        os << static_cast<int>(printer.value);
    }
    else if constexpr (std::is_same_v<std::int8_t, T>) {
        os << static_cast<int>(printer.value);
    }
    else {
        os << printer.value;
    }

    return os;
}

template <typename T>
std::ostream & operator<<(std::ostream & os, Printer<std::optional<T>> printer)
{
    if (const auto val = printer.value; val) {
        os << make_printer(*val);
    }
    else {
        os << "<empty>";
    }

    return os;
}

} // namespace blink {
