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

} // namespace blink::tests
