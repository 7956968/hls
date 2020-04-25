#include "hls/m3u8/Default_parser_factory.h"
#include "hls/m3u8/Parser_element_stream.h"
#include "hls/m3u8/String_line_reader.h"
#include "hls/playlist/master/Parser.h"

#include "iwu/test/Test_common.h"

class TestMasterPlaylist : public ::testing::Test {
public:
    std::unique_ptr<hls::m3u8::Parser_element_stream> create_stream(
      const std::string& source) {
        return std::make_unique<hls::m3u8::Parser_element_stream>(
          std::make_unique<hls::m3u8::String_line_reader>(source),
          hls::m3u8::Default_parser_factory{}.create());
    }

    std::unique_ptr<hls::playlist::master::Playlist> parse(
      const std::string& input) {
        auto stream{create_stream(input)};

        return hls::playlist::master::Parser::parse(stream.get(), ""s);
    }
};

TEST_F(TestMasterPlaylist, Empty) { ASSERT_THROW(parse(""), hls::Parse_error); }

TEST_F(TestMasterPlaylist, Basic) {
    std::unique_ptr<hls::playlist::master::Playlist> pl{
      parse("#EXTM3U\n"
            "#EXT-X-STREAM-INF:BANDWIDTH=1280000,AVERAGE-BANDWIDTH=1000000\n"
            "http://example.com/low.m3u8\n"
            "#EXT-X-STREAM-INF:BANDWIDTH=2560000,AVERAGE-BANDWIDTH=2000000\n"
            "http://example.com/mid.m3u8\n")};

    std::vector<const hls::playlist::master::IVariant_stream*> streams{
      pl->streams()};

    ASSERT_EQ(2, streams.size());

    ASSERT_EQ(streams[0]->uri().uri(), "http://example.com/low.m3u8");
    ASSERT_EQ(streams[0]->stream_inf().bandwidth(), 1280000);

    ASSERT_EQ(streams[1]->uri().uri(), "http://example.com/mid.m3u8");
    ASSERT_EQ(streams[1]->stream_inf().bandwidth(), 2560000);
}

TEST_F(TestMasterPlaylist, AlternateAudio) {
    std::unique_ptr<hls::playlist::master::Playlist> pl{parse(
      "#EXTM3U\n"
      "#EXT-X-MEDIA:TYPE=AUDIO,GROUP-ID=\"aac\",NAME=\"English\",DEFAULT=YES,"
      "AUTOSELECT=YES,LANGUAGE=\"en\", URI=\"main/english-audio.m3u8\"\n"
      "#EXT-X-MEDIA:TYPE=AUDIO,GROUP-ID=\"aac\",NAME=\"Deutsch\",DEFAULT=NO,"
      "AUTOSELECT=YES,LANGUAGE=\"de\",URI=\"main/german-audio.m3u8\"\n"
      "#EXT-X-STREAM-INF:BANDWIDTH=1280000,AUDIO=\"aac\"\n"
      "low/video-only.m3u8\n"
      "#EXT-X-STREAM-INF:BANDWIDTH=65000,CODECS=\"mp4a.40.5\",AUDIO=\"aac\"\n"
      "main/english-audio.m3u8\n")};

    std::vector<const hls::playlist::master::IVariant_stream*> streams{
      pl->streams()};

    ASSERT_EQ(2, streams.size());

    ASSERT_TRUE(
      streams[0]->get_rendition_group(hls::m3u8::Media_tag::Media_type::audio));
    ASSERT_TRUE(
      streams[1]->get_rendition_group(hls::m3u8::Media_tag::Media_type::audio));

    ASSERT_EQ(
      streams[0]->get_rendition_group(hls::m3u8::Media_tag::Media_type::audio),
      streams[1]->get_rendition_group(hls::m3u8::Media_tag::Media_type::audio));

    ASSERT_FALSE(
      streams[0]->get_rendition_group(hls::m3u8::Media_tag::Media_type::video));
    ASSERT_FALSE(streams[0]->get_rendition_group(
      hls::m3u8::Media_tag::Media_type::subtitles));
    ASSERT_FALSE(streams[0]->get_rendition_group(
      hls::m3u8::Media_tag::Media_type::closed_captions));

    {
        const hls::playlist::master::IRendition_group* rg{
          streams[0]->get_rendition_group(
            hls::m3u8::Media_tag::Media_type::audio)};

        ASSERT_EQ(hls::m3u8::Media_tag::Media_type::audio, rg->type());
        std::vector<const hls::m3u8::Media_tag*> media{rg->tags()};

        ASSERT_EQ(media.size(), 2);
    }
}

TEST_F(TestMasterPlaylist, AlternateVideo) {
    std::unique_ptr<hls::playlist::master::Playlist> pl{parse(
      // clang-format off
"#EXTM3U\n"
"#EXT-X-MEDIA:TYPE=VIDEO,GROUP-ID=\"low\",NAME=\"Main\",DEFAULT=YES,URI=\"low/main/audio-video.m3u8\"\n"
"#EXT-X-MEDIA:TYPE=VIDEO,GROUP-ID=\"low\",NAME=\"Centerfield\",DEFAULT=NO,URI=\"low/centerfield/audio-video.m3u8\"\n"
"#EXT-X-MEDIA:TYPE=VIDEO,GROUP-ID=\"low\",NAME=\"Dugout\",DEFAULT=NO,URI=\"low/dugout/audio-video.m3u8\"\n"
"#EXT-X-STREAM-INF:BANDWIDTH=1280000,CODECS=\"...\",VIDEO=\"low\"\n"
"low/main/audio-video.m3u8\n"
"#EXT-X-MEDIA:TYPE=VIDEO,GROUP-ID=\"mid\",NAME=\"Main\",DEFAULT=YES,URI=\"mid/main/audio-video.m3u8\"\n"
"#EXT-X-MEDIA:TYPE=VIDEO,GROUP-ID=\"mid\",NAME=\"Centerfield\",DEFAULT=NO,URI=\"mid/centerfield/audio-video.m3u8\"\n"
"#EXT-X-MEDIA:TYPE=VIDEO,GROUP-ID=\"mid\",NAME=\"Dugout\",DEFAULT=NO,URI=\"mid/dugout/audio-video.m3u8\"\n"
"#EXT-X-STREAM-INF:BANDWIDTH=2560000,CODECS=\"...\",VIDEO=\"mid\"\n"
"mid/main/audio-video.m3u8\n"
"#EXT-X-MEDIA:TYPE=VIDEO,GROUP-ID=\"hi\",NAME=\"Main\",DEFAULT=YES,URI=\"hi/main/audio-video.m3u8\"\n"
"#EXT-X-MEDIA:TYPE=VIDEO,GROUP-ID=\"hi\",NAME=\"Centerfield\",DEFAULT=NO,URI=\"hi/centerfield/audio-video.m3u8\"\n"
"#EXT-X-MEDIA:TYPE=VIDEO,GROUP-ID=\"hi\",NAME=\"Dugout\",DEFAULT=NO,URI=\"hi/dugout/audio-video.m3u8\"\n"
"#EXT-X-STREAM-INF:BANDWIDTH=7680000,CODECS=\"...\",VIDEO=\"hi\"\n"
"hi/main/audio-video.m3u8\n"
      // clang-format on
      )};

    std::vector<const hls::playlist::master::IVariant_stream*> streams{
      pl->streams()};
}
