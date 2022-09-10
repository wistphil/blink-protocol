#pragma once

#include "blink/DynamicGroupPreamble.hpp"
#include "blink/StaticGroupImpl.hpp"
#include "blink/Util.hpp"

namespace blink {

class DynamicGroupImpl
{
public:
    DynamicGroupImpl(
            std::uint64_t type_id,
            std::size_t data_area_offset,
            std::span<std::uint8_t> data);

    auto get_type_id() const -> std::uint64_t { return preamble_.get_type_id(); }
    auto size() const -> std::size_t { return size_; }

    template <typename T, typename... Args>
    void set_field(InlineStorage, std::size_t offset, T value, Args... args)
    { static_group_.set_field(offset, value, args...); }

    template <std::size_t N>
    void set_field(IndirectStorage, std::size_t offset, const char (& value)[N]);
    void set_field(IndirectStorage, std::size_t offset, std::string_view value);

    template <typename T>
    auto get_field(InlineStorage, std::size_t offset) -> T
    { return static_group_.get_field<T>(offset); }

    template <typename T>
    auto get_field(IndirectStorage storage_tag, std::size_t offset) -> T
    { return do_get_field(storage_tag, offset, Tag<T>{}); }

private:
    auto set_relative_offset(std::size_t offset) -> std::size_t;

    auto do_get_field(IndirectStorage, std::size_t offset, Tag<std::string_view> tag) const -> decltype(tag)::type;

private:
    DynamicGroupPreamble preamble_;
    StaticGroupImpl static_group_;
    std::span<std::uint8_t> data_area_;
    std::size_t data_area_offset_{0};
    std::size_t size_{0};
};

template <std::size_t N>
void DynamicGroupImpl::set_field(IndirectStorage storage_tag, std::size_t offset, const char (& value)[N])
{
    set_field(storage_tag, offset, std::string_view(value, N - 1));
}

} // namespace blink {
