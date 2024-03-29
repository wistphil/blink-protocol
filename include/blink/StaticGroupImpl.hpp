#pragma once

#include "blink/Util.hpp"

#include <optional>
#include <string_view>
#include <span>

namespace blink {

class StaticGroupImpl
{
public:
    StaticGroupImpl(std::span<std::uint8_t> data);

    auto data() const -> const std::uint8_t * { return data_.data(); }
    auto size() const -> std::size_t { return data_.size(); }

    auto subspan(std::size_t offset, std::size_t size) const -> std::span<std::uint8_t>
    { return data_.subspan(offset, size); }

    void set_inline_field(std::size_t offset, bool value);
    void set_inline_field(std::size_t offset, std::int8_t value);
    void set_inline_field(std::size_t offset, std::uint8_t value);
    void set_inline_field(std::size_t offset, std::int16_t value);
    void set_inline_field(std::size_t offset, std::uint16_t value);
    void set_inline_field(std::size_t offset, std::int32_t value);
    void set_inline_field(std::size_t offset, std::uint32_t value);
    void set_inline_field(std::size_t offset, std::int64_t value);
    void set_inline_field(std::size_t offset, std::uint64_t value);
    void set_inline_field(std::size_t offset, double value);

    void set_inline_field(std::size_t offset, std::string_view value, std::size_t max_length);
    void set_inline_field(std::size_t offset, std::span<std::uint8_t> value, std::size_t max_length);

    template <typename T, typename... Args>
    void set_inline_field(std::size_t offset, std::optional<T> value, Args... args);

    template <typename T>
    auto get_inline_field(std::size_t offset) const -> T
    { return do_get_inline_field(offset, Tag<T>{}); }

private:
    void do_set_inline_field(std::size_t offset, const void * ptr, std::size_t size, std::size_t max_length);

private:
    auto do_get_inline_field(std::size_t offset, Tag<bool> tag) const -> decltype(tag)::type;
    auto do_get_inline_field(std::size_t offset, Tag<std::int8_t> tag) const -> decltype(tag)::type;
    auto do_get_inline_field(std::size_t offset, Tag<std::uint8_t> tag) const -> decltype(tag)::type;
    auto do_get_inline_field(std::size_t offset, Tag<std::int16_t> tag) const -> decltype(tag)::type;
    auto do_get_inline_field(std::size_t offset, Tag<std::uint16_t> tag) const -> decltype(tag)::type;
    auto do_get_inline_field(std::size_t offset, Tag<std::int32_t> tag) const -> decltype(tag)::type;
    auto do_get_inline_field(std::size_t offset, Tag<std::uint32_t> tag) const -> decltype(tag)::type;
    auto do_get_inline_field(std::size_t offset, Tag<std::int64_t> tag) const -> decltype(tag)::type;
    auto do_get_inline_field(std::size_t offset, Tag<std::uint64_t> tag) const -> decltype(tag)::type;
    auto do_get_inline_field(std::size_t offset, Tag<double> tag) const -> decltype(tag)::type;

    auto do_get_inline_field(std::size_t offset, Tag<std::string_view> tag) const -> decltype(tag)::type;
    auto do_get_inline_field(std::size_t offset, Tag<std::span<std::uint8_t>> tag) const -> decltype(tag)::type;

    template <typename T>
    auto do_get_inline_field(std::size_t offset, Tag<std::optional<T>> tag) const -> typename decltype(tag)::type;

private:
    std::span<std::uint8_t> data_;
};

template<typename T, typename... Args>
void StaticGroupImpl::set_inline_field(std::size_t offset, std::optional<T> value, Args... args)
{
    if (value) {
        set_inline_field(offset++, true);
        set_inline_field(offset, *value, args...);
    }
    else {
        set_inline_field(offset, false);
    }
}

template <typename T>
auto StaticGroupImpl::do_get_inline_field(std::size_t offset, Tag<std::optional<T>> tag) const -> typename decltype(tag)::type
{
    if (do_get_inline_field(offset, Tag<bool>{})) {
        return do_get_inline_field(offset + 1, Tag<T>{});
    }
    return {};
}

} // namespace blink {
