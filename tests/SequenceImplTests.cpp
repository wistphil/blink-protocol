#include <gtest/gtest.h>

#include "blink/DataArea.hpp"
#include "blink/GroupImpl.hpp"
#include "blink/SequenceImpl.hpp"

#include <vector>

namespace blink::tests {

TEST(SequenceImplTests, construction)
{
    std::vector<std::uint8_t> data(32);
    std::span<std::uint8_t> inline_data(data.begin(), 4);
    DataArea data_area(std::span<std::uint8_t>(data.begin() + 4, data.end()));
    GroupImpl group(inline_data, 4, data_area);
    auto sequence = group.init_sequence(0, 4, 5);
    EXPECT_EQ(5, sequence.size());
}

TEST(SequenceImplTests, integer_sequence)
{
    std::vector<std::uint8_t> data(32);
    std::span<std::uint8_t> inline_data(data.begin(), 4);
    DataArea data_area(std::span<std::uint8_t>(data.begin() + 4, data.end()));
    GroupImpl group(inline_data, 4, data_area);
    EXPECT_EQ(5, group.init_sequence(0, 4, 5).size());

    auto sequence = group.get_sequence(0, 4);
    EXPECT_EQ(5, sequence.size());
    sequence.get(0).set_inline_field(0, 1);
    sequence.get(1).set_inline_field(0, 2);
    sequence.get(2).set_inline_field(0, 3);
    sequence.get(3).set_inline_field(0, 4);
    sequence.get(4).set_inline_field(0, 5);

    for (std::size_t i{0}; i < sequence.size(); ++i) {
        EXPECT_EQ(i + 1, sequence.get(i).get_inline_field<std::int32_t>(0));
    }
}

TEST(SequenceImplTests, string_sequence)
{
    std::vector<std::uint8_t> data(64);
    std::span<std::uint8_t> inline_data(data.begin(), 8);
    DataArea data_area(std::span<std::uint8_t>(data.begin() + 8, data.end()));
    GroupImpl group(inline_data, 8, data_area);
    EXPECT_EQ(3, group.init_sequence(4, 4, 3).size());

    auto sequence = group.get_sequence(4, 4);
    EXPECT_EQ(3, sequence.size());
    sequence.get(1).set_indirect_field(0, "one");
    sequence.get(0).set_indirect_field(0, "zero");
    group.set_indirect_field(0, "hello");
    sequence.get(2).set_indirect_field(0, "two");

    EXPECT_EQ(std::string_view("zero"), sequence.get(0).get_indirect_field<std::string_view>(0));
    EXPECT_EQ(std::string_view("one"), sequence.get(1).get_indirect_field<std::string_view>(0));
    EXPECT_EQ(std::string_view("two"), sequence.get(2).get_indirect_field<std::string_view>(0));
    EXPECT_EQ(std::string_view("hello"), group.get_indirect_field<std::string_view>(0));
}

} // namespace blink::tests
