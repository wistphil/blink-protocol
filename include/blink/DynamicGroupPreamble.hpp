#pragma once

#include <cstdint>
#include <span>

namespace blink {

class DynamicGroupPreamble
{
public:
    static constexpr std::size_t size{16};

    DynamicGroupPreamble(std::span<std::uint8_t> data);

    auto get_size() const -> std::uint32_t;
    void set_size(std::uint32_t value);

    auto get_type_id() const -> std::uint64_t;
    void set_type_id(std::uint64_t value);

    auto get_extension_offset() const -> std::uint32_t;
    void set_extension_offset(std::uint32_t value);

private:
    std::span<std::uint8_t> data_;
};

} // namespace blink {
