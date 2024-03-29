#pragma once

#include "blink/DataArea.hpp"
#include "blink/DynamicGroupPreamble.hpp"
#include "blink/GroupImpl.hpp"

namespace blink {

class SequenceImpl;

class DynamicGroupImpl
{
public:
    DynamicGroupImpl(
            std::uint64_t type_id,
            std::size_t data_area_offset,
            std::span<std::uint8_t> data);

    auto get_preamble() const -> DynamicGroupPreamble { return preamble_; }
    auto set_preamble_size() -> void { set_preamble_size(size()); }
    auto size() const -> std::size_t;
    auto data() const -> const std::uint8_t * { return data_; }

    template <typename T, typename... Args>
    auto set_inline_field(std::size_t offset, T value, Args... args) -> void
    {
        group_.set_inline_field(offset, value, args...);
    }

    template <typename T>
    auto set_indirect_field(std::size_t offset, T value) -> void
    {
        group_.set_indirect_field(offset, value);
    }

    template <typename T>
    auto get_inline_field(std::size_t offset) const -> T
    {
        return group_.get_inline_field<T>(offset);
    }

    template <typename T>
    auto get_indirect_field(std::size_t offset) const -> T
    {
        return group_.get_indirect_field<T>(offset);
    }

    auto get_group(std::size_t offset, std::size_t size) const -> GroupImpl;
    auto init_sequence(std::size_t offset, std::size_t group_size, std::size_t count) -> SequenceImpl;
    auto get_sequence(std::size_t offset, std::size_t group_size) const -> SequenceImpl;

private:
    auto set_preamble_size(std::size_t size) -> void;

private:
    DynamicGroupPreamble preamble_;
    DataArea data_area_;
    GroupImpl group_;
    std::uint8_t * data_{nullptr};
};

} // namespace blink {
