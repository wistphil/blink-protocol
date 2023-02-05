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
        TypeInfo type_info{.representation = "string", .type = FieldType::String};
        Field expected{.name = "make", .id = {}, .type_info = type_info, .inline_size = 11,  .max_length = 10, .is_optional = false};
        EXPECT_EQ(expected, fields[0]);
    }

    {
        TypeInfo type_info{.representation = "string", .type = FieldType::String};
        Field expected{.name = "model", .id = {}, .type_info = type_info, .inline_size = 4,  .indirect_size = 4, .max_length = {}, .is_optional = false};
        EXPECT_EQ(expected, fields[1]);
    }

    {
        TypeInfo type_info{.representation = "i32", .type = FieldType::I32};
        Field expected{.name = "year", .id = 42, .type_info = type_info, .inline_size = 4,  .max_length = {}, .is_optional = false};
        EXPECT_EQ(expected, fields[2]);
    }

    {
        TypeInfo type_info{.representation = "i32", .type = FieldType::I32};
        Field expected{.name = "miles", .id = {}, .type_info = type_info, .inline_size = 5,  .max_length = {}, .is_optional = true};
        EXPECT_EQ(expected, fields[3]);
    }
}

} // namespace blink::tests
