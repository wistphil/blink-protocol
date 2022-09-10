#include "blink/DynamicGroupImpl.hpp"

#include "byte_order/Codec.hpp"

namespace blink {

DynamicGroupImpl::DynamicGroupImpl(
        std::uint64_t type_id,
        std::size_t data_area_offset,
        std::span<std::uint8_t> data)
    : preamble_(data.subspan(0, DynamicGroupPreamble::size))
    , static_group_(data.subspan(DynamicGroupPreamble::size, data_area_offset))
    , data_area_(data.subspan(DynamicGroupPreamble::size + data_area_offset))
    , data_area_offset_(data_area_offset)
    , size_(data_area_offset)
{
    preamble_.set_size(static_cast<std::uint32_t>(size_));
    preamble_.set_type_id(type_id);
}

auto DynamicGroupImpl::set_relative_offset(std::size_t offset) -> std::size_t
{
    const auto relative_offset = static_cast<std::uint32_t>(size_ - offset);
    static_group_.set_field(offset, relative_offset);

    return static_cast<std::size_t>(offset + relative_offset - data_area_offset_);
}

void DynamicGroupImpl::set_field(IndirectStorage, std::size_t offset, std::string_view value)
{
    offset = set_relative_offset(offset);

    byte_order::encode_little(&data_area_[offset], static_cast<std::uint32_t>(value.size()));
    offset += sizeof(std::uint32_t);
    std::memcpy(&data_area_[offset], value.data(), value.size());

    size_ += value.size();
    preamble_.set_size(static_cast<std::uint32_t>(size_));
}

auto DynamicGroupImpl::do_get_field(IndirectStorage, std::size_t offset, Tag<std::string_view> tag) const -> decltype(tag)::type
{
    const auto relative_offset = static_group_.get_field<std::uint32_t>(offset);

    offset = static_cast<std::size_t>(offset + relative_offset - data_area_offset_);
    auto length = byte_order::decode_little<std::uint32_t>(&data_area_[offset]);
    offset += sizeof(std::uint32_t);
    const char * ptr = reinterpret_cast<const char *>(&data_area_[offset]);
    return std::string_view(ptr, length);
}

} // namespace blink {
