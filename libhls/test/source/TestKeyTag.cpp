#include "hls/m3u8/Key_tag.h"
#include "iwu/test/Test_common.h"

class TestKeyTag : public ::testing::Test {};

TEST_F(TestKeyTag, Empty) { ASSERT_THROW(hls::m3u8::Key_tag{""}, hls::Error); }

TEST_F(TestKeyTag, MethodNone) {
    hls::m3u8::Key_tag tag{"METHOD=NONE"};

    ASSERT_EQ(tag.method(), hls::m3u8::Key_tag::Method::none);
}

TEST_F(TestKeyTag, MethodAes) {
    hls::m3u8::Key_tag tag{"METHOD=AES-128,URI=\"http://test\""};

    ASSERT_EQ(tag.method(), hls::m3u8::Key_tag::Method::aes_128);
}

TEST_F(TestKeyTag, MethodSampleAes) {
    hls::m3u8::Key_tag tag{"METHOD=SAMPLE-AES,URI=\"http://test\""};

    ASSERT_EQ(tag.method(), hls::m3u8::Key_tag::Method::sample_aes);
}

TEST_F(TestKeyTag, Full) {
    hls::m3u8::Key_tag tag{"METHOD=AES-128,URI=\"http://"
                           "test\",IV=\"12345\",KEYFORMAT=\"keyformat\","
                           "KEYFORMATVERSIONS=\"keyformatversions\""};

    ASSERT_EQ(tag.uri().value(), "http://test"s);
    ASSERT_EQ(tag.iv().value(), "12345"s);
    ASSERT_EQ(tag.key_format().value(), "keyformat"s);
    ASSERT_EQ(tag.key_format_versions().value(), "keyformatversions"s);
    ASSERT_EQ(tag.method(), hls::m3u8::Key_tag::Method::aes_128);
}

TEST_F(TestKeyTag, MissingUri) {
    ASSERT_THROW(hls::m3u8::Key_tag tag{"METHOD=AES-128"}, hls::Error);
}

TEST_F(TestKeyTag, InvalidMethod) {
    ASSERT_THROW(hls::m3u8::Key_tag{"METHOD=INVALID"}, hls::Error);
}