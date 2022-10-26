#pragma once

#include "blink/DataArea.hpp"
#include "blink/StaticGroupImpl.hpp"
#include "blink/Util.hpp"

namespace blink {

class GroupImpl : public StaticGroupImpl
{
private:
    using static_group = StaticGroupImpl;

public:
    GroupImpl(
            std::span<std::uint8_t> data,
            std::int64_t data_area_offset,
            DataArea & data_area);

    template <std::size_t N>
    auto set_indirect_field(std::size_t offset, const char (& value)[N]) -> void;
    auto set_indirect_field(std::size_t offset, std::string_view value) -> void;

    template <typename T>
    auto set_indirect_field(std::size_t offset, std::optional<T> value) -> void;

    template <typename T>
    auto get_indirect_field(std::size_t offset) const -> T
    { return do_get_indirect_field(offset, Tag<T>{}); }

    auto get_group(std::size_t offset, std::size_t size) const -> GroupImpl;

private:
    auto set_relative_offset(std::size_t offset) -> void;

    auto do_get_indirect_field(std::size_t offset, Tag<std::string_view> tag) const -> decltype(tag)::type;

    template <typename T>
    auto do_get_indirect_field(std::size_t offset, Tag<std::optional<T>> tag) const -> typename decltype(tag)::type;

private:
    std::int64_t data_area_offset_{0};
    DataArea & data_area_;
};

template <std::size_t N>
auto GroupImpl::set_indirect_field(std::size_t offset, const char (& value)[N]) -> void
{
    set_indirect_field(offset, std::string_view(value, N - 1));
}

template<typename T>
auto GroupImpl::set_indirect_field(std::size_t offset, std::optional<T> value) -> void
{
    if (value) {
        static_group::set_inline_field(offset++, true);
        set_indirect_field(offset, *value);
    }
    else {
        static_group::set_inline_field(offset, false);
    }
}

template <typename T>
auto GroupImpl::do_get_indirect_field(std::size_t offset, Tag<std::optional<T>> tag) const -> typename decltype(tag)::type
{
    if (static_group::get_inline_field<bool>(offset)) {
        return do_get_indirect_field(offset + 1, Tag<T>{});
    }
    return {};
}

} // namespace blink {
