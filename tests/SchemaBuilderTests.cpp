#include <gtest/gtest.h>

#include "blink/Lexer.hpp"
#include "blink/SchemaBuilder.hpp"

namespace blink::tests {

TEST(SchemaBuilderTests, get_message)
{
    std::string str{"Car/1 -> string (10) make, string model, i32 year/42, i32 miles?"};
    auto tokens = Lexer{CharReader(str)}.tokenize();

    SchemaBuilder schema_builder{tokens};
    auto messages = schema_builder.get_messages();

    ASSERT_EQ(1, messages.size());
    const auto & msg = messages[0];

    EXPECT_EQ("Car", msg.name);
    EXPECT_EQ(1, msg.id);

    ASSERT_EQ(4, msg.fields.size());
    const auto & fields = msg.fields;

    {
        Field expected{.name = "make", .id = {}, .type = FieldType::String, .max_length = 10, .is_optional = false};
        EXPECT_EQ(expected, fields[0]);
    }

    {
        Field expected{.name = "model", .id = {}, .type = FieldType::String, .max_length = {}, .is_optional = false};
        EXPECT_EQ(expected, fields[1]);
    }

    {
        Field expected{.name = "year", .id = 42, .type = FieldType::I32, .max_length = {}, .is_optional = false};
        EXPECT_EQ(expected, fields[2]);
    }

    {
        Field expected{.name = "miles", .id = {}, .type = FieldType::I32, .max_length = {}, .is_optional = true};
        EXPECT_EQ(expected, fields[3]);
    }
}

} // namespace blink::tests
