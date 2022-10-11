#include <gtest/gtest.h>

#include "generated/BlinkMessages.hpp"

namespace blink::tests {

TEST(BlinkcTests, integers)
{
    std::vector<std::uint8_t> vec(64);
    Integers msg(vec);

    msg.set_num_u8(std::numeric_limits<std::uint8_t>::max());
    msg.set_num_i8(std::numeric_limits<std::int8_t>::min());
    EXPECT_EQ(std::numeric_limits<std::uint8_t>::max(), msg.get_num_u8());
    EXPECT_EQ(std::numeric_limits<std::int8_t>::min(), msg.get_num_i8());

    msg.set_num_u16(std::numeric_limits<std::uint16_t>::max());
    msg.set_num_i16(std::numeric_limits<std::int16_t>::min());
    EXPECT_EQ(std::numeric_limits<std::uint16_t>::max(), msg.get_num_u16());
    EXPECT_EQ(std::numeric_limits<std::int16_t>::min(), msg.get_num_i16());

    msg.set_num_u32(std::numeric_limits<std::uint32_t>::max());
    msg.set_num_i32(std::numeric_limits<std::int32_t>::min());
    EXPECT_EQ(std::numeric_limits<std::uint32_t>::max(), msg.get_num_u32());
    EXPECT_EQ(std::numeric_limits<std::int32_t>::min(), msg.get_num_i32());

    msg.set_num_u64(std::numeric_limits<std::uint64_t>::max());
    msg.set_num_i64(std::numeric_limits<std::int64_t>::min());
    EXPECT_EQ(std::numeric_limits<std::uint64_t>::max(), msg.get_num_u64());
    EXPECT_EQ(std::numeric_limits<std::int64_t>::min(), msg.get_num_i64());
}

TEST(BlinkcTests, optional_fields)
{
    std::vector<std::uint8_t> vec(64);
    OptFields msg(vec);

    EXPECT_FALSE(msg.get_num());
    msg.set_num(42);
    EXPECT_TRUE(msg.get_num());
    EXPECT_EQ(42, *msg.get_num());

    auto b = msg.get_b();
    EXPECT_FALSE(b.has_value());
    msg.set_b(false);
    EXPECT_TRUE(msg.get_b());
    EXPECT_FALSE(*msg.get_b());

    EXPECT_FALSE(msg.get_str1());
    msg.set_str1("123456");
    EXPECT_TRUE(msg.get_str1());
    EXPECT_EQ("123456", *msg.get_str1());

    EXPECT_FALSE(msg.get_str2());
    msg.set_str2("ABCDEFG");
    EXPECT_TRUE(msg.get_str2());
    EXPECT_EQ("ABCDEFG", *msg.get_str2());
}

} // namespace blink::tests
