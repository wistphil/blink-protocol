#include <gtest/gtest.h>

#include "blink/DynamicGroupImpl.hpp"

#include <vector>

namespace blink::tests {

TEST(DynamicGroupImplTests, construction)
{
    std::vector<std::uint8_t> data(17);
    DynamicGroupImpl group(1, data.size() - DynamicGroupPreamble::size, data);

    EXPECT_EQ(1, group.get_preamble().get_type_id());
    EXPECT_EQ(data.size(), group.size());
}

TEST(DynamicGroupImplTests, set_get_string_view)
{
    std::vector<std::uint8_t> data(32);
    DynamicGroupImpl group(std::numeric_limits<std::uint64_t>::max(), 4, data); 

    const std::string msg("hello");
    group.set_field(IndirectStorage{}, 0, msg);
    EXPECT_EQ(msg, group.get_field<std::string_view>(IndirectStorage{}, 0));
}

} // namespace blink::tests
