#include <gtest/gtest.h>

#include "blink/GroupImpl.hpp"

#include <vector>

namespace blink::tests {

TEST(GroupImplTests, construction)
{
    std::vector<std::uint8_t> data(32);
    std::span<std::uint8_t> inline_data(data.begin(), 8);
    DataArea data_area(std::span<std::uint8_t>(data.begin() + 12, data.end()));
    GroupImpl group(inline_data, 12, data_area);

    EXPECT_EQ(8, group.size());
}

TEST(GroupImplTests, set_get_string_view)
{
    std::vector<std::uint8_t> data(32);
    std::span<std::uint8_t> inline_data(data.begin(), 8);
    DataArea data_area(std::span<std::uint8_t>(data.begin() + 12, data.end()));
    GroupImpl group(inline_data, 12, data_area); 

    const std::string msg("hello");
    group.set_indirect_field(0, msg);
    EXPECT_EQ(msg, group.get_indirect_field<std::string_view>(0));

    group.set_indirect_field(4, "ABCD");
    EXPECT_EQ(std::string_view("ABCD"), group.get_indirect_field<std::string_view>(4)); 
}

TEST(GroupImplTests, set_get_opt_string_view)
{
    std::vector<std::uint8_t> data(32);
    std::span<std::uint8_t> inline_data(data.begin(), 8);
    DataArea data_area(std::span<std::uint8_t>(data.begin() + 12, data.end()));
    GroupImpl group(inline_data, 12, data_area); 

    std::optional<std::string> msg;
    group.set_indirect_field(0, msg);
    EXPECT_TRUE(!group.get_indirect_field<std::optional<std::string_view>>(0).has_value());

    msg = "hello";
    group.set_indirect_field(0, msg);
    auto decoded = group.get_indirect_field<std::optional<std::string_view>>(0);
    ASSERT_TRUE(decoded.has_value());
    EXPECT_EQ(std::string_view("hello"), *decoded); 
}

TEST(GroupImplTests, get_group)
{
    std::vector<std::uint8_t> data(40);
    std::span<std::uint8_t> inline_data(data.begin(), 20);
    DataArea data_area(std::span<std::uint8_t>(data.begin() + 20, data.end()));
    GroupImpl group(inline_data, 20, data_area);
    EXPECT_EQ(20, group.size());

    group.set_inline_field(0, 5);
    group.set_indirect_field(16, "ABC");
    {
        auto point = group.get_group(4, 12);
        EXPECT_EQ(12, point.size());
        point.set_inline_field(0, 2);
        point.set_inline_field(4, 4);
        point.set_indirect_field(8, "123");
    }

    EXPECT_EQ(5, group.get_inline_field<std::int32_t>(0));
    EXPECT_EQ(std::string_view("ABC"), group.get_indirect_field<std::string_view>(16));
    EXPECT_EQ(2, group.get_group(4, 12).get_inline_field<std::int32_t>(0));
    EXPECT_EQ(4, group.get_group(4, 12).get_inline_field<std::int32_t>(4));
    EXPECT_EQ(std::string_view("123"), group.get_group(4, 12).get_indirect_field<std::string_view>(8));
}

} // namespace blink::tests
