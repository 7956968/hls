#include "hls/m3u8/Default_parser_factory.h"
#include "hls/m3u8/Parser_element_stream.h"
#include "hls/m3u8/String_line_reader.h"
#include "hls/playlist/media/Parser.h"

#include "iwu/test/Test_common.h"

class TestMediaPlaylist : public ::testing::Test {
public:
    std::unique_ptr<hls::m3u8::Parser_element_stream> create_stream(
      const std::string& source) {
        return std::make_unique<hls::m3u8::Parser_element_stream>(
          std::make_unique<hls::m3u8::String_line_reader>(source),
          hls::m3u8::Default_parser_factory{}.create());
    }

    std::unique_ptr<hls::playlist::media::Playlist> parse(
      const std::string& input) {
        auto stream{create_stream(input)};

        return hls::playlist::media::Parser::parse(stream.get(), ""s);
    }
};

TEST_F(TestMediaPlaylist, Minimal) {
    std::unique_ptr<hls::playlist::media::Playlist> pl{
      parse("#EXTM3U\n"
            "#EXT-X-TARGETDURATION:10")};

    // Default values
    ASSERT_FALSE(pl->independent_segments());
    ASSERT_FALSE(pl->time_offset());
    ASSERT_FALSE(pl->type());
    ASSERT_FALSE(pl->is_iframes_only());
    ASSERT_FALSE(pl->precise_offset());
}

TEST_F(TestMediaPlaylist, PlaylistTags) {
    std::unique_ptr<hls::playlist::media::Playlist> pl{
      parse("#EXTM3U\n"
            "#EXT-X-TARGETDURATION:10\n"
            "#EXT-X-PLAYLIST-TYPE:EVENT\n"
            "#EXT-X-INDEPENDENT-SEGMENTS\n"
            "#EXT-X-START:TIME-OFFSET=-42.0,PRECISE=YES\n"
            "#EXT-X-I-FRAMES-ONLY\n")};

    ASSERT_EQ(pl->target_duration(), 10s);
    ASSERT_TRUE(pl->is_iframes_only());
    ASSERT_EQ(pl->type().value(), hls::m3u8::Playlist_type::event);

    ASSERT_EQ(pl->time_offset().value(), -42s);
    ASSERT_TRUE(pl->precise_offset());

    ASSERT_TRUE(pl->independent_segments());
}

TEST_F(TestMediaPlaylist, Empty) { ASSERT_THROW(parse(""s), hls::Error); }

TEST_F(TestMediaPlaylist, TargetDurationMissing) {
    ASSERT_THROW(parse("#EXTM3U\n"), hls::Error);
}

TEST_F(TestMediaPlaylist, M3uTagMissing) {
    ASSERT_THROW(parse("#EXT-X-TARGETDURATION:10"), hls::Error);
}

TEST_F(TestMediaPlaylist, Continuity) {
    std::unique_ptr<hls::playlist::media::Playlist> pl{
      parse("#EXTM3U\n"
            "#EXT-X-TARGETDURATION:10\n"
            "#EXTINF:9.009,\n"
            "http://media.example.com/first.ts\n"
            "#EXTINF:9.009,\n"
            "http://media.example.com/second.ts\n"
            "#EXTINF:3.003,\n"
            "#EXT-X-DISCONTINUITY\n"
            "http://media.example.com/third.ts\n"
            "#EXT-X-ENDLIST\n")};

    const std::vector<hls::playlist::media::Segment>& segments{pl->segments()};
    ASSERT_EQ(3, segments.size());

    ASSERT_EQ(segments[0].sequence_number(), 0);
    ASSERT_EQ(segments[1].sequence_number(), 1);
    ASSERT_EQ(segments[2].sequence_number(), 2);

    ASSERT_EQ(segments[0].discontinuity_sequence_number(), 0);
    ASSERT_EQ(segments[1].discontinuity_sequence_number(), 0);
    ASSERT_EQ(segments[2].discontinuity_sequence_number(), 1);
}

TEST_F(TestMediaPlaylist, ContinuityTag) {
    std::unique_ptr<hls::playlist::media::Playlist> pl{
      parse("#EXTM3U\n"
            "#EXT-X-TARGETDURATION:10\n"
            "#EXT-X-MEDIA-SEQUENCE:42\n"
            "#EXT-X-DISCONTINUITY-SEQUENCE:84\n"
            "#EXTINF:9.009,\n"
            "http://media.example.com/first.ts\n"
            "#EXTINF:9.009,\n"
            "http://media.example.com/second.ts\n"
            "#EXTINF:3.003,\n"
            "#EXT-X-DISCONTINUITY\n"
            "http://media.example.com/third.ts\n"
            "#EXT-X-ENDLIST\n")};

    const std::vector<hls::playlist::media::Segment>& segments{pl->segments()};
    ASSERT_EQ(3, segments.size());

    ASSERT_EQ(segments[0].sequence_number(), 42);
    ASSERT_EQ(segments[1].sequence_number(), 43);
    ASSERT_EQ(segments[2].sequence_number(), 44);

    ASSERT_EQ(segments[0].discontinuity_sequence_number(), 84);
    ASSERT_EQ(segments[1].discontinuity_sequence_number(), 84);
    ASSERT_EQ(segments[2].discontinuity_sequence_number(), 85);
}

TEST_F(TestMediaPlaylist, Segments) {
    std::unique_ptr<hls::playlist::media::Playlist> pl{
      parse("#EXTM3U\n"
            "#EXT-X-TARGETDURATION:10\n"
            "#EXTINF:9.009\n"
            "http://media.example.com/first.ts\n"
            "#EXTINF:9.009,title\n"
            "#EXT-X-GAP\n"
            "#EXT-X-BYTERANGE:42@1234\n"
            "#EXT-X-PROGRAM-DATE-TIME:2017-06-09T04:59:01.797Z\n"
            "http://media.example.com/second.ts\n"
            "#EXTINF:3.003\n"
            "http://media.example.com/third.ts\n"
            "#EXT-X-ENDLIST\n")};

    const std::vector<hls::playlist::media::Segment>& segments{pl->segments()};
    ASSERT_EQ(3, segments.size());

    ASSERT_EQ(segments[0].uri(), "http://media.example.com/first.ts");
    ASSERT_EQ(segments[0].duration(), 9009ms);
    ASSERT_FALSE(segments[0].title());
    ASSERT_FALSE(segments[0].is_gap());
    ASSERT_FALSE(segments[0].byte_range());
    ASSERT_FALSE(segments[0].program_date_time());

    ASSERT_EQ(segments[1].uri(), "http://media.example.com/second.ts");
    ASSERT_EQ(segments[1].duration(), 9009ms);
    ASSERT_EQ(segments[1].title().value(), "title");
    ASSERT_TRUE(segments[1].is_gap());
    ASSERT_TRUE(segments[1].byte_range());
    ASSERT_TRUE(segments[1].program_date_time());

    ASSERT_EQ(segments[2].uri(), "http://media.example.com/third.ts");
    ASSERT_EQ(segments[2].duration(), 3003ms);
    ASSERT_FALSE(segments[2].title());
    ASSERT_FALSE(segments[2].is_gap());
    ASSERT_FALSE(segments[2].byte_range());
    ASSERT_FALSE(segments[2].program_date_time());
}


TEST_F(TestMediaPlaylist, SharedTags) {
    std::unique_ptr<hls::playlist::media::Playlist> pl{
      parse("#EXTM3U\n"
            "#EXT-X-TARGETDURATION:10\n"
            "#EXTINF:9.009,\n"
            "http://media.example.com/first.ts\n"
            "#EXT-X-BITRATE:12345\n"
            "#EXT-X-KEY:METHOD=NONE\n"
            "#EXT-X-MAP:URI=\"http://test\"\n"
            "#EXTINF:9.009,\n"
            "http://media.example.com/second.ts\n"
            "#EXTINF:3.003,\n"
            "http://media.example.com/third.ts\n"
            "#EXT-X-ENDLIST\n")};

    const std::vector<hls::playlist::media::Segment>& segments{pl->segments()};
    ASSERT_EQ(3, segments.size());

    ASSERT_FALSE(segments[0].bitrate());
    ASSERT_FALSE(segments[0].key());
    ASSERT_FALSE(segments[0].map());

    ASSERT_EQ(segments[1].bitrate(), 12345);
    ASSERT_TRUE(segments[1].map());
    ASSERT_TRUE(segments[1].key());

    ASSERT_EQ(segments[2].bitrate(), 12345);
    ASSERT_TRUE(segments[2].map());
    ASSERT_TRUE(segments[2].key());
}
