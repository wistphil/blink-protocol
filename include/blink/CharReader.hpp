#pragma once

#include <span>

namespace blink {

class CharReader
{
public:
    explicit CharReader(std::span<const char> ref)
        : ref_(ref)
    { }

    auto increment() -> void
    {
        ++current_index_;
    }

    auto current() const -> const char &
    {
        return ref_[current_index_];
    }

    auto peek(int i) const -> const char &
    {
        const std::size_t index = static_cast<std::size_t>(i);
        return ref_[current_index_ + index];
    }

    auto is_end(int i = 0) const -> bool
    {
        return current_index_ + static_cast<std::size_t>(i) >= ref_.size();
    } 

private:
    std::span<const char> ref_; 
    std::size_t current_index_{0};
};

} // namespace blink {
