#pragma once

#include "blink/Util.hpp"

#include <span>

namespace blink {

class DataArea
{
public:
    explicit DataArea(std::span<std::uint8_t> data);

    auto size() const -> std::size_t { return size_; }

    auto add_field(std::string_view value) -> void; 

    template <typename T>
    auto get_field(std::size_t offset) const -> T
    { return do_get_field(offset, Tag<T>{}); }

private:
    auto do_get_field(std::size_t offset, Tag<std::string_view> tag) const -> decltype(tag)::type;

private:
    std::span<std::uint8_t> data_;
    std::size_t size_{0};
};

} // namespace blink {
