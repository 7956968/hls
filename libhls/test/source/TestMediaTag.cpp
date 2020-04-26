
#include "hls/m3u8/Media_tag.h"

#include "iwu/test/Test_common.h"

class TestMediaTag : public ::testing::Test {};

TEST_F(TestMediaTag, Empty) {
    ASSERT_THROW(hls::m3u8::Media_tag("", nullptr), hls::Error);
}

TEST_F(TestMediaTag, Minimal) {
    hls::m3u8::Media_tag{"TYPE=VIDEO,GROUP-ID=\"G\",NAME=\"N\"", nullptr};
}

TEST_F(TestMediaTag, Type) {
    {
        ASSERT_EQ(
          hls::m3u8::Media_tag("TYPE=VIDEO,GROUP-ID=\"G\",NAME=\"N\"", nullptr)
            .media_type(),
          hls::m3u8::Media_tag::Media_type::video);
    }

    {
        ASSERT_EQ(
          hls::m3u8::Media_tag("TYPE=AUDIO,GROUP-ID=\"G\",NAME=\"N\"", nullptr)
            .media_type(),
          hls::m3u8::Media_tag::Media_type::audio);
    }

    {
        ASSERT_EQ(hls::m3u8::Media_tag(
                    "TYPE=SUBTITLES,GROUP-ID=\"G\",NAME=\"N\"", nullptr)
                    .media_type(),
                  hls::m3u8::Media_tag::Media_type::subtitles);
    }

    {
        ASSERT_EQ(hls::m3u8::Media_tag(
                    "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\"", nullptr)
                    .media_type(),
                  hls::m3u8::Media_tag::Media_type::closed_captions);
    }

    // Invalid
    ASSERT_THROW(
      hls::m3u8::Media_tag("TYPE=invalid,GROUP-ID=\"G\",NAME=\"N\"", nullptr),
      hls::Error);
    // Missing
    ASSERT_THROW(hls::m3u8::Media_tag("GROUP-ID=\"G\",NAME=\"N\"", nullptr),
                 hls::Error);
}

TEST_F(TestMediaTag, GroupId) {
    ASSERT_THROW(hls::m3u8::Media_tag("TYPE=VIDEO,NAME=\"N\"", nullptr),
                 hls::Error);

    ASSERT_EQ(hls::m3u8::Media_tag(
                "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"GID\",NAME=\"N\"", nullptr)
                .group_id(),
              "GID"s);
}

TEST_F(TestMediaTag, Name) {
    ASSERT_THROW(hls::m3u8::Media_tag("TYPE=VIDEO,GROUP-ID=\"G\"", nullptr),
                 hls::Error);

    ASSERT_EQ(hls::m3u8::Media_tag(
                "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"GID\",NAME=\"name\"", nullptr)
                .name(),
              "name"s);
}

TEST_F(TestMediaTag, Uri) {
    ASSERT_FALSE(
      hls::m3u8::Media_tag("TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\"", nullptr)
        .uri());

    ASSERT_EQ(
      hls::m3u8::Media_tag(
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",URI=\"uri\"", nullptr)
        .uri()
        .value(),
      "uri"s);
}

TEST_F(TestMediaTag, Language) {
    ASSERT_FALSE(
      hls::m3u8::Media_tag("TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\"", nullptr)
        .language());

    ASSERT_EQ(
      hls::m3u8::Media_tag(
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",LANGUAGE=\"eng\"",
        nullptr)
        .language()
        .value(),
      "eng"s);
}

TEST_F(TestMediaTag, AssocLanguage) {
    ASSERT_FALSE(
      hls::m3u8::Media_tag("TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\"", nullptr)
        .assoc_language());

    ASSERT_EQ(
      hls::m3u8::Media_tag(
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",ASSOC-LANGUAGE=\"eng\"",
        nullptr)
        .assoc_language()
        .value(),
      "eng"s);
}

TEST_F(TestMediaTag, Default) {
    ASSERT_FALSE(
      hls::m3u8::Media_tag("TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\"", nullptr)
        .is_default());

    ASSERT_TRUE(
      hls::m3u8::Media_tag(
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",DEFAULT=YES", nullptr)
        .is_default());

    ASSERT_FALSE(
      hls::m3u8::Media_tag(
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",DEFAULT=NO", nullptr)
        .is_default());
}

TEST_F(TestMediaTag, AutoSelect) {
    ASSERT_FALSE(
      hls::m3u8::Media_tag("TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\"", nullptr)
        .auto_select());

    ASSERT_TRUE(
      hls::m3u8::Media_tag(
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",AUTOSELECT=YES",
        nullptr)
        .auto_select());

    ASSERT_FALSE(
      hls::m3u8::Media_tag(
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",AUTOSELECT=NO", nullptr)
        .auto_select());
}

TEST_F(TestMediaTag, Forced) {
    ASSERT_FALSE(
      hls::m3u8::Media_tag("TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\"", nullptr)
        .forced());

    ASSERT_TRUE(
      hls::m3u8::Media_tag(
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",FORCED=YES", nullptr)
        .forced());

    ASSERT_FALSE(
      hls::m3u8::Media_tag(
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",FORCED=NO", nullptr)
        .forced());
}

TEST_F(TestMediaTag, InStreamId) {
    ASSERT_FALSE(
      hls::m3u8::Media_tag("TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\"", nullptr)
        .in_stream_id());

    ASSERT_EQ(
      hls::m3u8::Media_tag(
        "TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME=\"N\",INSTREAM-ID=\"isid\"",
        nullptr)
        .in_stream_id()
        .value(),
      "isid"s);
}

TEST_F(TestMediaTag, Characteristics) {
    ASSERT_FALSE(
      hls::m3u8::Media_tag("TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\"", nullptr)
        .characteristics());

    std::vector<std::string> characteristics{
      hls::m3u8::Media_tag("TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME="
                           "\"N\",CHARACTERISTICS=\"one,two,three\"",
                           nullptr)
        .characteristics()
        .value()};
    ASSERT_EQ(characteristics.size(), 3);
    ASSERT_EQ(characteristics[0], "one");
    ASSERT_EQ(characteristics[1], "two");
    ASSERT_EQ(characteristics[2], "three");
}

TEST_F(TestMediaTag, Channels) {
    ASSERT_FALSE(
      hls::m3u8::Media_tag("TYPE=VIDEO,NAME=\"N\",GROUP-ID=\"G\"", nullptr)
        .channels());

    std::vector<std::string> channels{
      hls::m3u8::Media_tag("TYPE=CLOSED-CAPTIONS,GROUP-ID=\"G\",NAME="
                           "\"N\",CHANNELS=\"one/two/three\"",
                           nullptr)
        .channels()
        .value()};
    ASSERT_EQ(channels.size(), 3);
    ASSERT_EQ(channels[0], "one");
    ASSERT_EQ(channels[1], "two");
    ASSERT_EQ(channels[2], "three");
}