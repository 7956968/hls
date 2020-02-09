
#include "Common.h"

#include "hls/m3u8/Integer_tag.h"

class TestIntegerTag : public ::testing::Test {};

TEST_F(TestIntegerTag, Valid) {
    const hls::m3u8::Integer_tag tag{"56", hls::m3u8::Tag::Tag_type::inf};

    ASSERT_EQ(tag.value(), 56);
    ASSERT_EQ(tag.type(), hls::m3u8::Tag::Tag_type::inf);
}

TEST_F(TestIntegerTag, Empty) {
    ASSERT_THROW((hls::m3u8::Integer_tag{"", hls::m3u8::Tag::Tag_type::inf}),
                 hls::Error);
}

TEST_F(TestIntegerTag, Invalid) {
    ASSERT_THROW(
      (hls::m3u8::Integer_tag{"invalid", hls::m3u8::Tag::Tag_type::inf}),
      hls::Error);
}
