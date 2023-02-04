#pragma once

#include "blink/GroupImpl.hpp"
#include "blink/SequenceImpl.hpp"

namespace blink {

template <typename T>
class Sequence
{
public:
    Sequence(SequenceImpl impl)
        : impl_(impl)
    {}

    auto size() const -> std::size_t { return impl_.size(); }
    auto get(std::size_t index) const -> T { return T(impl_.get(index)); }

    auto operator[](std::size_t index) const -> T { return get(index); }

private:
    SequenceImpl impl_;
};

} // namespace blink {
