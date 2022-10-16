#pragma once

#include "blink/DynamicGroupPreamble.hpp"
#include "blink/StaticGroupImpl.hpp"
#include "blink/Util.hpp"

namespace blink {

class DynamicGroupImpl : public StaticGroupImpl
{
private:
    using static_group = StaticGroupImpl;

public:
    DynamicGroupImpl(
            std::uint64_t type_id,
            std::size_t data_area_offset,
            std::span<std::uint8_t> data);

    auto get_preamble() const -> DynamicGroupPreamble { return preamble_; }
    auto size() const -> std::size_t { return size_; }
    auto data() const -> const std::uint8_t * { return data_; }

    template <std::size_t N>
    auto set_indirect_field(std::size_t offset, const char (& value)[N]) -> void;
    auto set_indirect_field(std::size_t offset, std::string_view value) -> void;

    template <typename T>
    auto set_indirect_field(std::size_t offset, std::optional<T> value) -> void;

    template <typename T>
    auto get_indirect_field(std::size_t offset) const -> T
    { return do_get_indirect_field(offset, Tag<T>{}); }

private:
    auto set_preamble_size(std::size_t size) -> void;
    auto set_relative_offset(std::size_t offset) -> std::size_t;

    auto do_get_indirect_field(std::size_t offset, Tag<std::string_view> tag) const -> decltype(tag)::type;

    template <typename T>
    auto do_get_indirect_field(std::size_t offset, Tag<std::optional<T>> tag) const -> typename decltype(tag)::type;

private:
    DynamicGroupPreamble preamble_;
    std::span<std::uint8_t> data_area_;
    std::size_t data_area_offset_{0};
    std::size_t size_{0};
    std::uint8_t * data_{nullptr};
};

template <std::size_t N>
auto DynamicGroupImpl::set_indirect_field(std::size_t offset, const char (& value)[N]) -> void
{
    set_indirect_field(offset, std::string_view(value, N - 1));
}

template<typename T>
auto DynamicGroupImpl::set_indirect_field(std::size_t offset, std::optional<T> value) -> void
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
auto DynamicGroupImpl::do_get_indirect_field(std::size_t offset, Tag<std::optional<T>> tag) const -> typename decltype(tag)::type
{
    if (static_group::get_inline_field<bool>(offset)) {
        return do_get_indirect_field(offset + 1, Tag<T>{});
    }
    return {};
}

} // namespace blink {
