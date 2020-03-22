#include "hls/m3u8/Byte_range.h"
#include "hls/m3u8/Common.h"
#include "hls/m3u8/Enum_tag.h"
#include "hls/m3u8/Inf_tag.h"
#include "hls/m3u8/Integer_tag.h"
#include "hls/m3u8/Map_tag.h"

#include "iwu/test/Test_common.h"

class TestTags : public ::testing::Test {};

TEST_F(TestTags, Integer) {
    // Valid
    {
        const hls::m3u8::Integer_tag tag{"56", hls::m3u8::Tag::Tag_type::inf};

        ASSERT_EQ(tag.value(), 56);
        ASSERT_EQ(tag.type(), hls::m3u8::Tag::Tag_type::inf);
    }

    // Empty
    {
        ASSERT_THROW(
          (hls::m3u8::Integer_tag{"", hls::m3u8::Tag::Tag_type::inf}),
          hls::Error);
    }

    // Invalid
    {
        ASSERT_THROW(
          (hls::m3u8::Integer_tag{"invalid", hls::m3u8::Tag::Tag_type::inf}),
          hls::Error);
    }
}


enum class Test_enum { one, two };

hls::m3u8::Enum_parser_fnc<Test_enum> test_enum_parser{
  [](const std::string& name) {
      return hls::m3u8::parse_enum_string(
        std::map<std::string, Test_enum>{{"one"s, Test_enum::one},
                                         {"two"s, Test_enum::two}},
        name);
  }};

TEST_F(TestTags, EnumTag) {
    ASSERT_EQ((hls::m3u8::Enum_tag<Test_enum>{
                "one", hls::m3u8::Tag::Tag_type::unknown, test_enum_parser}
                 .value()),
              Test_enum::one);

    ASSERT_EQ((hls::m3u8::Enum_tag<Test_enum>{
                "two", hls::m3u8::Tag::Tag_type::unknown, test_enum_parser}
                 .value()),
              Test_enum::two);

    ASSERT_THROW((hls::m3u8::Enum_tag<Test_enum>{
                   "three", hls::m3u8::Tag::Tag_type::unknown, test_enum_parser}
                    .value()),
                 hls::Error);
}

TEST_F(TestTags, InfTag) {
    EXPECT_DOUBLE_EQ(5, hls::m3u8::Inf_tag{"5"}.duration());

    EXPECT_DOUBLE_EQ(5.45, hls::m3u8::Inf_tag{"5.45"}.duration());

    EXPECT_THROW(hls::m3u8::Inf_tag{""}.duration(), hls::Parse_error);
    EXPECT_THROW(hls::m3u8::Inf_tag{"asdf"}.duration(), hls::Parse_error);
    EXPECT_THROW(hls::m3u8::Inf_tag{",title"}.duration(), hls::Parse_error);

    EXPECT_DOUBLE_EQ(5, hls::m3u8::Inf_tag{"5,"}.duration());

    {
        hls::m3u8::Inf_tag tag{"5,title"};

        EXPECT_DOUBLE_EQ(tag.duration(), 5);
        EXPECT_EQ("title"s, tag.title().value());
    }

    {
        hls::m3u8::Inf_tag tag{"5,"};

        EXPECT_DOUBLE_EQ(tag.duration(), 5);
        EXPECT_EQ(""s, tag.title().value());
    }
}

// TODO Move to a separate file ?
TEST_F(TestTags, ByteRange) {
    ASSERT_THROW(hls::m3u8::Byte_range{""}, iwu::Precondition_violation);
    ASSERT_THROW(hls::m3u8::Byte_range{"asdf"}, iwu::Precondition_violation);
    ASSERT_THROW(hls::m3u8::Byte_range{"@"}, iwu::Precondition_violation);
    ASSERT_THROW(hls::m3u8::Byte_range{"@345"}, iwu::Precondition_violation);
    ASSERT_THROW(hls::m3u8::Byte_range{"123123@"}, iwu::Precondition_violation);
    ASSERT_THROW(hls::m3u8::Byte_range{"asdf@123"},
                 iwu::Precondition_violation);
    ASSERT_THROW(hls::m3u8::Byte_range{"123@asdf"},
                 iwu::Precondition_violation);


    {
        hls::m3u8::Byte_range br{"42"};

        ASSERT_EQ(42, br.length());
        ASSERT_FALSE(br.start_byte().has_value());
    }

    {
        hls::m3u8::Byte_range br{"42@1234"};

        ASSERT_EQ(42, br.length());
        ASSERT_EQ(1234, br.start_byte().value());
    }
}

TEST_F(TestTags, MapTag) {
    ASSERT_ANY_THROW(hls::m3u8::Map_tag{""});

    {
        hls::m3u8::Map_tag tag{"URI=\"http://test\""};
        ASSERT_EQ(tag.uri(), "http://test");
        ASSERT_FALSE(tag.byte_range().has_value());
    }

    {
        hls::m3u8::Map_tag tag{"URI=\"http://test\",BYTERANGE=\"1234@52\""};
        ASSERT_EQ(tag.uri(), "http://test");
        ASSERT_EQ(tag.byte_range().value().length(), 1234);
        ASSERT_EQ(tag.byte_range().value().start_byte(), 52);
    }
}
