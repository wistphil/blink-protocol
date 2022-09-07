#pragma once

#include <optional>
#include <string_view>
#include <span>

namespace blink {

class StaticGroupImpl
{
    template <typename T>
    struct Tag { using type = T; };

public:
    StaticGroupImpl(std::span<std::uint8_t> data);

    void set_field(std::size_t offset, bool value);
    void set_field(std::size_t offset, std::int8_t value);
    void set_field(std::size_t offset, std::uint8_t value);
    void set_field(std::size_t offset, std::int16_t value);
    void set_field(std::size_t offset, std::uint16_t value);
    void set_field(std::size_t offset, std::int32_t value);
    void set_field(std::size_t offset, std::uint32_t value);
    void set_field(std::size_t offset, std::int64_t value);
    void set_field(std::size_t offset, std::uint64_t value);
    void set_field(std::size_t offset, double value);

    void set_field(std::size_t offset, std::string_view value, std::size_t max_length);
    void set_field(std::size_t offset, std::span<std::uint8_t> value, std::size_t max_length);

    template<typename T>
    void set_field(std::size_t offset, std::optional<T> value);

    template <typename T>
    auto get_field(std::size_t offset) const -> T
    { return get_field(offset, Tag<T>{}); }

private:
    void set_field(std::size_t offset, const void * ptr, std::size_t size, std::size_t max_length);

private:
    auto get_field(std::size_t offset, Tag<bool> tag) const -> decltype(tag)::type;
    auto get_field(std::size_t offset, Tag<std::int8_t> tag) const -> decltype(tag)::type;
    auto get_field(std::size_t offset, Tag<std::uint8_t> tag) const -> decltype(tag)::type;
    auto get_field(std::size_t offset, Tag<std::int16_t> tag) const -> decltype(tag)::type;
    auto get_field(std::size_t offset, Tag<std::uint16_t> tag) const -> decltype(tag)::type;
    auto get_field(std::size_t offset, Tag<std::int32_t> tag) const -> decltype(tag)::type;
    auto get_field(std::size_t offset, Tag<std::uint32_t> tag) const -> decltype(tag)::type;
    auto get_field(std::size_t offset, Tag<std::int64_t> tag) const -> decltype(tag)::type;
    auto get_field(std::size_t offset, Tag<std::uint64_t> tag) const -> decltype(tag)::type;
    auto get_field(std::size_t offset, Tag<double> tag) const -> decltype(tag)::type;

    auto get_field(std::size_t offset, Tag<std::string_view> tag) const -> decltype(tag)::type;
    auto get_field(std::size_t offset, Tag<std::span<std::uint8_t>> tag) const -> decltype(tag)::type;

    template <typename T>
    auto get_field(std::size_t offset, Tag<std::optional<T>> tag) const -> typename decltype(tag)::type;

private:
    std::span<std::uint8_t> data_;
};

template<typename T>
void StaticGroupImpl::set_field(std::size_t offset, std::optional<T> value)
{
    if (value) {
        set_field(offset++, true);
        set_field(offset, *value);
    }
    else {
        set_field(offset, false);
    }
}

template <typename T>
auto StaticGroupImpl::get_field(std::size_t offset, Tag<std::optional<T>> tag) const -> typename decltype(tag)::type
{
    if (get_field(offset, Tag<bool>{})) {
        return get_field(offset + 1, Tag<T>{});
    }
    return {};
}

} // namespace blink {
