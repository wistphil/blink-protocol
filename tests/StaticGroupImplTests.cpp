#include <gtest/gtest.h>

#include "blink/StaticGroupImpl.hpp"

#include <vector>

namespace blink::tests {

template <typename T>
auto set_field_get_field(T value, std::size_t buffer_size = sizeof(T)) -> T
{
    std::vector<std::uint8_t> data(buffer_size);
    StaticGroupImpl group(data);

    group.set_field(0, value);
    return group.get_field<T>(0);
}

TEST(StaticGroupImplTests, set_get_bool)
{
    EXPECT_EQ(true, set_field_get_field(true));
    EXPECT_EQ(false, set_field_get_field(false));
}

TEST(StaticGroupImplTests, set_get_int8_t)
{
    std::int8_t value{std::numeric_limits<std::int8_t>::min()};
    EXPECT_EQ(value, set_field_get_field(value));
}

TEST(StaticGroupImplTests, set_get_uint8_t)
{
    std::uint8_t value{std::numeric_limits<std::uint8_t>::max()};
    EXPECT_EQ(value, set_field_get_field(value));
}

TEST(StaticGroupImplTests, set_get_int16_t)
{
    std::int16_t value{std::numeric_limits<std::int16_t>::min()};
    EXPECT_EQ(value, set_field_get_field(value));
}

TEST(StaticGroupImplTests, set_get_uint16_t)
{
    std::uint16_t value{std::numeric_limits<std::uint16_t>::max()};
    EXPECT_EQ(value, set_field_get_field(value));
}

TEST(StaticGroupImplTests, set_get_int32_t)
{
    std::int32_t value{std::numeric_limits<std::int32_t>::min()};
    EXPECT_EQ(value, set_field_get_field(value));
}

TEST(StaticGroupImplTests, set_get_uint32_t)
{
    std::uint32_t value{std::numeric_limits<std::uint32_t>::max()};
    EXPECT_EQ(value, set_field_get_field(value));
}

TEST(StaticGroupImplTests, set_get_int64_t)
{
    std::int64_t value{std::numeric_limits<std::int64_t>::min()};
    EXPECT_EQ(value, set_field_get_field(value));
}

TEST(StaticGroupImplTests, set_get_uint64_t)
{
    std::uint64_t value{std::numeric_limits<std::uint64_t>::max()};
    EXPECT_EQ(value, set_field_get_field(value));
}

TEST(StaticGroupImplTests, set_get_double)
{
    double value{std::numeric_limits<double>::min()};
    EXPECT_DOUBLE_EQ(value, set_field_get_field(value));

    value = std::numeric_limits<double>::max();
    EXPECT_DOUBLE_EQ(value, set_field_get_field(value));
}

TEST(StaticGroupImplTests, set_get_string_view)
{
    std::vector<std::uint8_t> data(6);
    StaticGroupImpl group(data);

    std::string msg("hello");
    group.set_field(0, msg, 5);
    EXPECT_EQ(msg, group.get_field<std::string_view>(0));

    std::vector<std::uint8_t> expected{5, 'h', 'e', 'l', 'l', 'o'};
    EXPECT_EQ(expected, data);
}

TEST(StaticGroupImplTests, set_get_string_view_with_trailing_nulls)
{
    std::vector<std::uint8_t> data(11);
    StaticGroupImpl group(data);

    std::string msg("hello");
    group.set_field(0, msg, 10);
    EXPECT_EQ(msg, group.get_field<std::string_view>(0));

    std::vector<std::uint8_t> expected{5, 'h', 'e', 'l', 'l', 'o', '\0', '\0', '\0', '\0', '\0'};
    EXPECT_EQ(expected, data);
}

TEST(StaticGroupImplTests, set_get_binary)
{
    std::vector<std::uint8_t> data(6);
    StaticGroupImpl group(data);

    std::vector<std::uint8_t> value{1, 2, 0, 4, 5};
    group.set_field(0, value, 5);
    auto decoded = group.get_field<std::span<std::uint8_t>>(0);
    EXPECT_TRUE(std::equal(value.begin(), value.end(), decoded.begin()));

    std::vector<std::uint8_t> expected{5, 1, 2, 0, 4, 5};
    EXPECT_EQ(expected, data);
}

TEST(StaticGroupImplTests, set_get_binary_with_trailing_nulls)
{
    std::vector<std::uint8_t> data(11);
    StaticGroupImpl group(data);

    std::vector<std::uint8_t> value{1, 2, 0, 4, 5};
    group.set_field(0, value, 10);
    auto decoded = group.get_field<std::span<std::uint8_t>>(0);
    EXPECT_TRUE(std::equal(value.begin(), value.end(), decoded.begin()));

    std::vector<std::uint8_t> expected{5, 1, 2, 0, 4, 5, 0, 0, 0, 0, 0};
    EXPECT_EQ(expected, data);
}

TEST(StaticGroupImplTests, set_get_optional)
{
    std::optional<std::int32_t> value;
    EXPECT_EQ(value, set_field_get_field(value, sizeof(value) + 1));

    value = 128;
    EXPECT_EQ(value, set_field_get_field(value, sizeof(value) + 1));
}

} // namespace blink::tests
