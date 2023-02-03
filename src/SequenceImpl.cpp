#include "blink/SequenceImpl.hpp"

#include "blink/DataArea.hpp"
#include "blink/GroupImpl.hpp"

namespace blink {

SequenceImpl::SequenceImpl(
        std::size_t offset,
        std::size_t group_size,
        DataArea & data_area)
    : offset_(offset)
    , group_size_(group_size) 
    , data_area_(data_area)
{
}

auto SequenceImpl::size() const -> std::size_t
{
    return data_area_.get_field<std::uint32_t>(offset_);
}

auto SequenceImpl::get(std::size_t index) const -> GroupImpl
{
    const std::size_t offset = index * group_size_ + offset_ + sizeof(std::uint32_t);

    if (index >= size() || offset >= data_area_.size()) {
        // 
    }
    
    auto group_data = data_area_.subspan(offset, group_size_);
    const std::int64_t data_area_offset = static_cast<std::int64_t>(-offset);
    return GroupImpl(group_data, data_area_offset, data_area_);
}

auto SequenceImpl::operator[](std::size_t index) const -> GroupImpl
{
    return get(index);
}

} // namespace blink {
