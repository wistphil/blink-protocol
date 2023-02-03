#include "blink/GroupImpl.hpp"

#include "blink/DataArea.hpp"
#include "blink/SequenceImpl.hpp"

namespace blink {

GroupImpl::GroupImpl(
        std::span<std::uint8_t> data,
        std::int64_t data_area_offset,
        DataArea & data_area)
    : static_group(data)
    , data_area_offset_(data_area_offset)
    , data_area_(data_area)
    { }

auto GroupImpl::set_relative_offset(std::size_t offset) -> void
{
    const std::int64_t data_area_offset = data_area_offset_ - static_cast<std::int64_t>(offset);
    const std::int64_t relative_offset = data_area_offset + static_cast<std::int64_t>(data_area_.size());
    static_group::set_inline_field(offset, static_cast<std::uint32_t>(relative_offset));
}

auto GroupImpl::set_indirect_field(std::size_t offset, std::string_view value) -> void
{
    // return if field is already set
    if (static_group::get_inline_field<std::uint32_t>(offset) != 0) {
        return;
    }

    set_relative_offset(offset);
    data_area_.add_field(value);
}

auto GroupImpl::do_get_indirect_field(const std::size_t offset, Tag<std::string_view> tag) const -> decltype(tag)::type
{
    const auto relative_offset = static_group::get_inline_field<std::uint32_t>(offset);

    const std::int64_t new_offset = static_cast<std::int64_t>(relative_offset) - (data_area_offset_ - static_cast<std::int64_t>(offset));
    return data_area_.get_field<decltype(tag)::type>(static_cast<std::size_t>(new_offset));
}

auto GroupImpl::get_group(std::size_t offset, std::size_t size) const -> GroupImpl
{
    auto group_data = static_group::subspan(offset, size);
    const std::int64_t data_area_offset = data_area_offset_ - static_cast<std::int64_t>(offset);
    return GroupImpl(group_data, data_area_offset, data_area_);
}

auto GroupImpl::init_sequence(std::size_t offset, std::size_t group_size, std::size_t count) -> SequenceImpl
{
    set_relative_offset(offset);
    data_area_.add_field(static_cast<std::uint32_t>(count));
    data_area_.reserve(group_size * count);
    return get_sequence(offset, group_size);
}

auto GroupImpl::get_sequence(std::size_t offset, std::size_t group_size) const -> SequenceImpl
{
    const auto relative_offset = static_group::get_inline_field<std::uint32_t>(offset);

    const std::int64_t new_offset = static_cast<std::int64_t>(relative_offset) - (data_area_offset_ - static_cast<std::int64_t>(offset));
    return SequenceImpl(static_cast<size_t>(new_offset), group_size, data_area_);
}

} // namespace blink {
