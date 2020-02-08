
#include "Common.h"

#include "hls/m3u8/Attribute_list.h"

class TestM3u8AttributeList : public ::testing::Test {};

TEST_F(TestM3u8AttributeList, String) {
    const hls::m3u8::Attribute_list al{"string_key=\"string_value\""};

    ASSERT_EQ(al.get<std::string>("string_key"), "string_value");
}

TEST_F(TestM3u8AttributeList, Integer) {
    const hls::m3u8::Attribute_list al{"integer_key=42"};

    ASSERT_EQ(al.get<long long>("integer_key"), 42);
}

TEST_F(TestM3u8AttributeList, Float) {
    const hls::m3u8::Attribute_list al{"float_key=4.2"};

    ASSERT_FLOAT_EQ(al.get<float>("float_key"), 4.2);
}

TEST_F(TestM3u8AttributeList, Resolution) {
    const hls::m3u8::Attribute_list al{"resolution_key=120x320"};

    ASSERT_EQ(al.get<hls::m3u8::Attribute_list::Resolution>("resolution_key"),
              std::make_pair(120L, 320L));
}

TEST_F(TestM3u8AttributeList, EnumString) {
    const hls::m3u8::Attribute_list al{"enum_key=ENUM_VALUE"};

    ASSERT_EQ(al.get<hls::m3u8::Attribute_list::String_enum>("enum_key").value,
              "ENUM_VALUE"s);
}

TEST_F(TestM3u8AttributeList, Boolean) {
    const hls::m3u8::Attribute_list al{
      "bool_key_true=YES,bool_key_false=NO,bool_key_invalid=apple"};

    ASSERT_TRUE(al.get<bool>("bool_key_true"));
    ASSERT_FALSE(al.get<bool>("bool_key_false"));

    ASSERT_THROW(al.get<bool>("bool_key_invalid"), hls::Error);
}

TEST_F(TestM3u8AttributeList, Empty) { const hls::m3u8::Attribute_list al{""}; }

TEST_F(TestM3u8AttributeList, Invalid) {
    ASSERT_THROW(const hls::m3u8::Attribute_list al{",!@(#*&!@#"}, hls::Error);
}

TEST_F(TestM3u8AttributeList, MultipleAttributes) {
    const hls::m3u8::Attribute_list al{
      "string_key=\"test string value\""
      ",decimal_key=42,float_key=4.2"
      ",resolution_key=120x320,enum_key=ENUM_VALUE"};

    ASSERT_EQ(al.get<std::string>("string_key"), "test string value");
    ASSERT_EQ(al.get<long long>("decimal_key"), 42L);
    ASSERT_FLOAT_EQ(al.get<float>("float_key"), 4.2);
    ASSERT_EQ(al.get<hls::m3u8::Attribute_list::Resolution>("resolution_key"),
              std::make_pair(120L, 320L));
    ASSERT_EQ(al.get<hls::m3u8::Attribute_list::String_enum>("enum_key").value,
              "ENUM_VALUE"s);
}
