#pragma once

#include <cstddef>

namespace blink {

class DataArea;
class GroupImpl;

class SequenceImpl
{
public:
    SequenceImpl(
            std::size_t offset,
            std::size_t group_size,
            DataArea & data_area);

    auto size() const -> std::size_t;
    auto get(std::size_t index) const -> GroupImpl;

    auto operator[](std::size_t index) const -> GroupImpl;

private:
    const std::size_t offset_{0};
    const std::size_t size_{0};
    const std::size_t group_size_{0};
    DataArea & data_area_;
};

} // namespace blink {
