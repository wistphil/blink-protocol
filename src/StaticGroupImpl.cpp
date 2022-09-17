#include "blink/StaticGroupImpl.hpp"

#include "byte_order/Codec.hpp"

#include <limits>

namespace blink {

StaticGroupImpl::StaticGroupImpl(std::span<std::uint8_t> data)
    : data_(data)
{
}

void StaticGroupImpl::set_field(std::size_t offset, bool value)
{
    const std::uint8_t tmp = value == true ? 0x01 : 0x00;
    byte_order::encode_little(&data_[offset], tmp);
}

void StaticGroupImpl::set_field(std::size_t offset, std::int8_t value)
{
    byte_order::encode_little(&data_[offset], value);
}

void StaticGroupImpl::set_field(std::size_t offset, std::uint8_t value)
{
    byte_order::encode_little(&data_[offset], value);
}

void StaticGroupImpl::set_field(std::size_t offset, std::int16_t value)
{
    byte_order::encode_little(&data_[offset], value);
}

void StaticGroupImpl::set_field(std::size_t offset, std::uint16_t value)
{
    byte_order::encode_little(&data_[offset], value);
}

void StaticGroupImpl::set_field(std::size_t offset, std::int32_t value)
{
    byte_order::encode_little(&data_[offset], value);
}

void StaticGroupImpl::set_field(std::size_t offset, std::uint32_t value)
{
    byte_order::encode_little(&data_[offset], value);
}

void StaticGroupImpl::set_field(std::size_t offset, std::int64_t value)
{
    byte_order::encode_little(&data_[offset], value);
}

void StaticGroupImpl::set_field(std::size_t offset, std::uint64_t value)
{
    byte_order::encode_little(&data_[offset], value);
}

void StaticGroupImpl::set_field(std::size_t offset, std::string_view value, std::size_t max_length)
{
    do_set_field(offset, value.data(), value.size(), max_length);
}

void StaticGroupImpl::set_field(std::size_t offset, std::span<std::uint8_t> value, std::size_t max_length)
{
    do_set_field(offset, value.data(), value.size(), max_length);
}

void StaticGroupImpl::do_set_field(std::size_t offset, const void * ptr, std::size_t size, std::size_t max_length)
{
    const std::size_t length{std::min(size, max_length)};

    set_field(offset++, static_cast<std::uint8_t>(length));

    std::memcpy(&data_[offset], ptr, length);

    if (length < max_length) {
        std::memset(&data_[offset + length], '\0', max_length - length);
    }
}

void StaticGroupImpl::set_field(std::size_t offset, double value)
{
    static_assert(std::numeric_limits<decltype(value)>::is_iec559, "IEEE 754 floating point arithmetic is required");

    byte_order::encode_little(&data_[offset], value);
}

auto StaticGroupImpl::do_get_field(std::size_t offset, Tag<bool> tag) const -> decltype(tag)::type
{
    const auto tmp{do_get_field(offset, Tag<std::uint8_t>{})};
    return tmp == 0x01 ? true : false;
}

auto StaticGroupImpl::do_get_field(std::size_t offset, Tag<std::int8_t> tag) const -> decltype(tag)::type
{
    return byte_order::decode_little<decltype(tag)::type>(&data_[offset]);
}

auto StaticGroupImpl::do_get_field(std::size_t offset, Tag<std::uint8_t> tag) const -> decltype(tag)::type
{
    return byte_order::decode_little<decltype(tag)::type>(&data_[offset]);
}

auto StaticGroupImpl::do_get_field(std::size_t offset, Tag<std::int16_t> tag) const -> decltype(tag)::type
{
    return byte_order::decode_little<decltype(tag)::type>(&data_[offset]);
}

auto StaticGroupImpl::do_get_field(std::size_t offset, Tag<std::uint16_t> tag) const -> decltype(tag)::type
{
    return byte_order::decode_little<decltype(tag)::type>(&data_[offset]);
}

auto StaticGroupImpl::do_get_field(std::size_t offset, Tag<std::int32_t> tag) const -> decltype(tag)::type
{
    return byte_order::decode_little<decltype(tag)::type>(&data_[offset]);
}

auto StaticGroupImpl::do_get_field(std::size_t offset, Tag<std::uint32_t> tag) const -> decltype(tag)::type
{
    return byte_order::decode_little<decltype(tag)::type>(&data_[offset]);
}

auto StaticGroupImpl::do_get_field(std::size_t offset, Tag<std::int64_t> tag) const -> decltype(tag)::type
{
    return byte_order::decode_little<decltype(tag)::type>(&data_[offset]);
}

auto StaticGroupImpl::do_get_field(std::size_t offset, Tag<std::uint64_t> tag) const -> decltype(tag)::type
{
    return byte_order::decode_little<decltype(tag)::type>(&data_[offset]);
}

auto StaticGroupImpl::do_get_field(std::size_t offset, Tag<double> tag) const -> decltype(tag)::type
{
    using T = decltype(tag)::type;

    static_assert(std::numeric_limits<T>::is_iec559, "IEEE 754 floating point arithmetic is required");

    return byte_order::decode_little<decltype(tag)::type>(&data_[offset]);
}

auto StaticGroupImpl::do_get_field(std::size_t offset, Tag<std::string_view> tag) const -> decltype(tag)::type
{
    const auto length{do_get_field(offset++, Tag<std::uint8_t>{})};
    const char * ptr = reinterpret_cast<const char *>(&data_[offset]);
    return std::string_view(ptr, length);
}

auto StaticGroupImpl::do_get_field(std::size_t offset, Tag<std::span<std::uint8_t>> tag) const -> decltype(tag)::type
{
    const auto length{do_get_field(offset++, Tag<std::uint8_t>{})};
    return std::span<std::uint8_t>(&data_[offset], length);
}

} // namespace blink {
