#include "blink/DynamicGroupPreamble.hpp"

#include "byte_order/Codec.hpp"

namespace blink {

DynamicGroupPreamble::DynamicGroupPreamble(std::span<std::uint8_t> data)
    : data_(data.subspan(0, DynamicGroupPreamble::size))
{
}

auto DynamicGroupPreamble::get_size() const -> std::uint32_t
{
    return byte_order::decode_little<std::uint32_t>(&data_[0]);
}

void DynamicGroupPreamble::set_size(std::uint32_t value)
{
    byte_order::encode_little(&data_[0], value);
}

auto DynamicGroupPreamble::get_type_id() const -> std::uint64_t
{
    return byte_order::decode_little<std::uint64_t>(&data_[4]);
}

void DynamicGroupPreamble::set_type_id(std::uint64_t value)
{
    byte_order::encode_little(&data_[4], value);
}

auto DynamicGroupPreamble::get_extension_offset() const -> std::uint32_t
{
    return byte_order::decode_little<std::uint32_t>(&data_[12]);
}

void DynamicGroupPreamble::set_extension_offset(std::uint32_t value)
{
    byte_order::encode_little(&data_[12], value);
}

} // namespace blink {
