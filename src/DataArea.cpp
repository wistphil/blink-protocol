#include "blink/DataArea.hpp"

#include "byte_order/Codec.hpp"

namespace blink {

DataArea::DataArea(std::span<std::uint8_t> data)
    : data_(data)
{
}

auto DataArea::add_field(std::string_view value) -> void
{
    auto offset = static_cast<std::uint32_t>(size_);
    byte_order::encode_little(&data_[offset], static_cast<std::uint32_t>(value.size()));
    offset += sizeof(std::uint32_t);
    std::memcpy(&data_[offset], value.data(), value.size());

    size_ += (value.size() + sizeof(std::uint32_t));
}

auto DataArea::do_get_field(const std::size_t offset, Tag<std::string_view> tag) const -> decltype(tag)::type
{
    auto new_offset = offset;
    auto length = byte_order::decode_little<std::uint32_t>(&data_[new_offset]);
    new_offset += sizeof(std::uint32_t);
    const char * ptr = reinterpret_cast<const char *>(&data_[new_offset]);
    return std::string_view(ptr, length);
}

} // namespace blink {
