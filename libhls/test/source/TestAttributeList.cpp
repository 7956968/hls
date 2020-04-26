
#include "hls/m3u8/Attribute_list.h"

#include "iwu/test/Test_common.h"

class TestAttributeList : public ::testing::Test {};

TEST_F(TestAttributeList, String) {
    const hls::m3u8::Attribute_list al{"string_key=\"string_value\"", nullptr};

    ASSERT_EQ(al.get<std::string>("string_key"), "string_value");
}

TEST_F(TestAttributeList, Integer) {
    const hls::m3u8::Attribute_list al{"integer_key=42", nullptr};

    ASSERT_EQ(al.get<long long>("integer_key"), 42);
}

TEST_F(TestAttributeList, Float) {
    const hls::m3u8::Attribute_list al{"float_key=4.2", nullptr};

    ASSERT_FLOAT_EQ(al.get<float>("float_key"), 4.2);
}

TEST_F(TestAttributeList, Resolution) {
    const hls::m3u8::Attribute_list al{"resolution_key=120x320", nullptr};

    ASSERT_EQ(al.get<hls::m3u8::Attribute_list::Resolution>("resolution_key"),
              std::make_pair(120L, 320L));
}

TEST_F(TestAttributeList, EnumString) {
    const hls::m3u8::Attribute_list al{"enum_key=ENUM_VALUE", nullptr};

    ASSERT_EQ(al.get<hls::m3u8::Attribute_list::String_enum>("enum_key").value,
              "ENUM_VALUE"s);
}

TEST_F(TestAttributeList, Boolean) {
    const hls::m3u8::Attribute_list al{
      "bool_key_true=YES,bool_key_false=NO,bool_key_invalid=apple", nullptr};

    ASSERT_TRUE(al.get<bool>("bool_key_true"));
    ASSERT_FALSE(al.get<bool>("bool_key_false"));

    ASSERT_THROW(al.get<bool>("bool_key_invalid"), hls::Error);
}

TEST_F(TestAttributeList, Empty) {
    const hls::m3u8::Attribute_list al{"", nullptr};
}

TEST_F(TestAttributeList, Invalid) {
    ASSERT_THROW(hls::m3u8::Attribute_list(",!@(#*&!@#", nullptr), hls::Error);
}

TEST_F(TestAttributeList, MultipleAttributes) {
    const hls::m3u8::Attribute_list al{
      "string_key=\"test string value\""
      ",decimal_key=42,float_key=4.2"
      ",resolution_key=120x320,enum_key=ENUM_VALUE",
      nullptr};

    ASSERT_EQ(al.get<std::string>("string_key"), "test string value");
    ASSERT_EQ(al.get<long long>("decimal_key"), 42L);
    ASSERT_FLOAT_EQ(al.get<float>("float_key"), 4.2);
    ASSERT_EQ(al.get<hls::m3u8::Attribute_list::Resolution>("resolution_key"),
              std::make_pair(120L, 320L));
    ASSERT_EQ(al.get<hls::m3u8::Attribute_list::String_enum>("enum_key").value,
              "ENUM_VALUE"s);
}

TEST_F(TestAttributeList, Contains) {
    const hls::m3u8::Attribute_list al{"integer_key=42", nullptr};

    ASSERT_TRUE(al.contains("integer_key"));
    ASSERT_FALSE(al.contains("asdf"));
}

TEST_F(TestAttributeList, CSV) {
    const hls::m3u8::Attribute_list al{"csv_key=\"first,second,third\"",
                                       nullptr};


    std::vector<std::string> values{
      al.get_delim_separated_string("csv_key"s, ","s)};
    ASSERT_EQ(3, values.size());

    ASSERT_EQ(values[0], "first");
    ASSERT_EQ(values[1], "second");
    ASSERT_EQ(values[2], "third");
}
