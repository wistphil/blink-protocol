#include "blink/DynamicGroupImpl.hpp"

#include "blink/SequenceImpl.hpp"

namespace blink {

DynamicGroupImpl::DynamicGroupImpl(
        std::uint64_t type_id,
        std::size_t data_area_offset,
        std::span<std::uint8_t> data)
    : preamble_(data.subspan(0, DynamicGroupPreamble::size))
    , data_area_(data.subspan(DynamicGroupPreamble::size + data_area_offset))
    , group_(data.subspan(DynamicGroupPreamble::size, data_area_offset), static_cast<std::int64_t>(data_area_offset), data_area_)
    , data_(data.data())
{
    if (const auto preamble_size = preamble_.get_size(); preamble_size != 0) {
        const std::size_t inline_size = DynamicGroupPreamble::size + data_area_offset;
        const std::size_t total_size = preamble_size + sizeof(std::uint32_t);
        if (total_size > inline_size) {
            const auto data_area_size = total_size - inline_size;
            data_area_.reserve(data_area_size);
        }
    }

    set_preamble_size();
    preamble_.set_type_id(type_id);
}

auto DynamicGroupImpl::size() const -> std::size_t
{
    return DynamicGroupPreamble::size + group_.size() + data_area_.size();
}

auto DynamicGroupImpl::set_preamble_size(const std::size_t size) -> void
{
    // preamble 'size' does not include itself, so subtract from total size
    preamble_.set_size(static_cast<std::uint32_t>(size) - sizeof(std::uint32_t));
}

auto DynamicGroupImpl::get_group(std::size_t offset, std::size_t size) const -> GroupImpl
{
    return group_.get_group(offset, size);
}

auto DynamicGroupImpl::init_sequence(std::size_t offset, std::size_t group_size, std::size_t count) -> SequenceImpl
{
    auto sequence = group_.init_sequence(offset, group_size, count);
    return sequence;
}

auto DynamicGroupImpl::get_sequence(std::size_t offset, std::size_t group_size) const -> SequenceImpl
{
    return group_.get_sequence(offset, group_size);
}

} // namespace blink {
