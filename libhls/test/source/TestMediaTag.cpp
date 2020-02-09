
#include "Common.h"

#include "hls/m3u8/Media_tag.h"

class TestMediaTag : public ::testing::Test {};

TEST_F(TestMediaTag, Empty) {
    ASSERT_THROW(hls::m3u8::Media_tag{""}, hls::Error);
}

TEST_F(TestMediaTag, Minimal) {
    hls::m3u8::Media_tag{"TYPE=VIDEO,GROUP-ID=\"G\",NAME=\"N\""};
}

TEST_F(TestMediaTag, Type) {
    {
        ASSERT_EQ(hls::m3u8::Media_tag{"TYPE=VIDEO,GROUP-ID=\"G\",NAME=\"N\""}
                    .media_type(),
                  hls::m3u8::Media_tag::Media_type::video);
    }

    {
        ASSERT_EQ(hls::m3u8::Media_tag{"TYPE=AUDIO,GROUP-ID=\"G\",NAME=\"N\""}
                    .media_type(),
                  hls::m3u8::Media_tag::Media_type::audio);
    }

    {
        ASSERT_EQ(
          hls::m3u8::Media_tag{"TYPE=SUBTITLES,GROUP-ID=\"G\",NAME=\"N\""}
            .media_type(),
          hls::m3u8::Media_tag::Media_type::subtitles);
    }

    {
        ASSERT_EQ(
          hls::m3u8::Media_tag{"TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\""}
            .media_type(),
          hls::m3u8::Media_tag::Media_type::closed_captions);
    }

    // Invalid
    ASSERT_THROW(hls::m3u8::Media_tag{"TYPE=invalid,GROUP-ID=\"G\",NAME=\"N\""},
                 hls::Error);
    // Missing
    ASSERT_THROW(hls::m3u8::Media_tag{"GROUP-ID=\"G\",NAME=\"N\""}, hls::Error);
}

TEST_F(TestMediaTag, GroupId) {
    ASSERT_THROW(hls::m3u8::Media_tag{"TYPE=VIDEO,NAME=\"N\""}, hls::Error);

    ASSERT_EQ(
      hls::m3u8::Media_tag{"TYPE=CLOSED-CAPTIONS,GROUP-ID=\"GID\",NAME=\"N\""}
        .group_id(),
      "GID"s);
}

TEST_F(TestMediaTag, Name) {
    ASSERT_THROW(hls::m3u8::Media_tag{"TYPE=VIDEO,GROUP-ID=\"G\""}, hls::Error);

    ASSERT_EQ(
      hls::m3u8::Media_tag{
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"GID\",NAME=\"name\""}
        .name(),
      "name"s);
}

TEST_F(TestMediaTag, Uri) {
    ASSERT_TRUE(hls::m3u8::Media_tag{"TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\""}
                  .uri()
                  .empty());

    ASSERT_EQ(
      hls::m3u8::Media_tag{
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",URI=\"uri\""}
        .uri(),
      "uri"s);
}

TEST_F(TestMediaTag, Language) {
    ASSERT_TRUE(hls::m3u8::Media_tag{"TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\""}
                  .language()
                  .empty());

    ASSERT_EQ(
      hls::m3u8::Media_tag{
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",LANGUAGE=\"eng\""}
        .language(),
      "eng"s);
}

TEST_F(TestMediaTag, AssocLanguage) {
    ASSERT_TRUE(hls::m3u8::Media_tag{"TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\""}
                  .assoc_language()
                  .empty());

    ASSERT_EQ(
      hls::m3u8::Media_tag{
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",ASSOC-LANGUAGE=\"eng\""}
        .assoc_language(),
      "eng"s);
}

TEST_F(TestMediaTag, Default) {
    ASSERT_FALSE(hls::m3u8::Media_tag{"TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\""}
                   .is_default());

    ASSERT_TRUE(hls::m3u8::Media_tag{
      "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",DEFAULT=YES"}
                  .is_default());

    ASSERT_FALSE(hls::m3u8::Media_tag{
      "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",DEFAULT=NO"}
                   .is_default());
}

TEST_F(TestMediaTag, AutoSelect) {
    ASSERT_FALSE(hls::m3u8::Media_tag{"TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\""}
                   .auto_select());

    ASSERT_TRUE(hls::m3u8::Media_tag{
      "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",AUTOSELECT=YES"}
                  .auto_select());

    ASSERT_FALSE(hls::m3u8::Media_tag{
      "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",AUTOSELECT=NO"}
                   .auto_select());
}

TEST_F(TestMediaTag, Forced) {
    ASSERT_FALSE(
      hls::m3u8::Media_tag{"TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\""}.forced());

    ASSERT_TRUE(hls::m3u8::Media_tag{
      "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",FORCED=YES"}
                  .forced());

    ASSERT_FALSE(hls::m3u8::Media_tag{
      "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",FORCED=NO"}
                   .forced());
}

TEST_F(TestMediaTag, InStreamId) {
    ASSERT_TRUE(hls::m3u8::Media_tag{"TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\""}
                  .in_stream_id()
                  .empty());

    ASSERT_EQ(
      hls::m3u8::Media_tag{
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",INSTREAM-ID=\"isid\""}
        .in_stream_id(),
      "isid"s);
}

TEST_F(TestMediaTag, Characteristics) {
    ASSERT_TRUE(hls::m3u8::Media_tag{"TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\""}
                  .characteristics()
                  .empty());

    std::vector<std::string> characteristics{
      hls::m3u8::Media_tag{"TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME="
                           "\"N\",CHARACTERISTICS=\"one,two,three\""}
        .characteristics()};
    ASSERT_EQ(characteristics.size(), 3);
    ASSERT_EQ(characteristics[0], "one");
    ASSERT_EQ(characteristics[1], "two");
    ASSERT_EQ(characteristics[2], "three");
}

TEST_F(TestMediaTag, Channels) {
    ASSERT_TRUE(hls::m3u8::Media_tag{"TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\""}
                  .channels()
                  .empty());

    std::vector<std::string> channels{
      hls::m3u8::Media_tag{"TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME="
                           "\"N\",CHANNELS=\"one/two/three\""}
        .channels()};
    ASSERT_EQ(channels.size(), 3);
    ASSERT_EQ(channels[0], "one");
    ASSERT_EQ(channels[1], "two");
    ASSERT_EQ(channels[2], "three");
}