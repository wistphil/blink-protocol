#pragma once

#include "blink/StaticGroupImpl.hpp"
#include "blink/Util.hpp"

#include <span>

namespace blink {

class GroupImpl;

class DataArea
{
public:
    explicit DataArea(std::span<std::uint8_t> data);

    auto size() const -> std::size_t { return size_; }

    auto subspan(std::size_t offset, std::size_t size) const -> std::span<std::uint8_t>
    { return data_.subspan(offset, size); }

    auto reserve(std::size_t size) { size_ += size; }

    auto add_field(std::string_view value) -> void; 
    auto add_field(std::uint32_t value) -> void;

    template <typename T>
    auto get_field(std::size_t offset) const -> T
    { return do_get_field(offset, Tag<T>{}); }

private:
    auto do_get_field(std::size_t offset, Tag<std::string_view> tag) const -> decltype(tag)::type;

    template <typename T>
    auto do_get_field(std::size_t offset, Tag<T> tag) const -> typename decltype(tag)::type
    { return StaticGroupImpl{data_}.get_inline_field<T>(offset); }

private:
    std::span<std::uint8_t> data_;
    std::size_t size_{0};
};

} // namespace blink {
