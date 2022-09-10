#pragma once

namespace blink {

template <typename T>
struct Tag { using type = T; };

struct InlineStorage {};
struct IndirectStorage {};

} // namespace blink {
