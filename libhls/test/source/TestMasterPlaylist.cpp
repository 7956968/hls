#include "hls/m3u8/Default_parser_factory.h"
#include "hls/m3u8/Parser_element_stream.h"
#include "hls/m3u8/String_line_reader.h"
#include "hls/playlist/Master_playlist_parser.h"

#include "Common.h"

class TestMasterPlaylist : public ::testing::Test {
public:
    std::unique_ptr<hls::m3u8::Parser_element_stream> create_stream(
      const std::string& source) {
        return std::make_unique<hls::m3u8::Parser_element_stream>(
          std::make_unique<hls::m3u8::String_line_reader>(source),
          hls::m3u8::Default_parser_factory{}.create());
    }

    std::unique_ptr<hls::playlist::Master_playlist> parse(
      const std::string& input) {
        auto stream{create_stream(input)};

        return hls::playlist::Master_playlist_parser{stream.get()}.parse();
    }
};

TEST_F(TestMasterPlaylist, Empty) { ASSERT_THROW(parse(""), hls::Parse_error); }

TEST_F(TestMasterPlaylist, Basic) {
    std::unique_ptr<hls::playlist::Master_playlist> pl{
      parse("#EXTM3U\n"
            "#EXT-X-STREAM-INF:BANDWIDTH=1280000,AVERAGE-BANDWIDTH=1000000\n"
            "http://example.com/low.m3u8\n"
            "#EXT-X-STREAM-INF:BANDWIDTH=2560000,AVERAGE-BANDWIDTH=2000000\n"
            "http://example.com/mid.m3u8\n")};

    std::vector<const hls::playlist::IVariant_stream*> streams{pl->streams()};

    ASSERT_EQ(2, streams.size());

    ASSERT_EQ(streams[0]->uri().uri(), "http://example.com/low.m3u8");
    ASSERT_EQ(streams[0]->stream_inf().bandwidth(), 1280000);

    ASSERT_EQ(streams[1]->uri().uri(), "http://example.com/mid.m3u8");
    ASSERT_EQ(streams[1]->stream_inf().bandwidth(), 2560000);
}


TEST_F(TestMasterPlaylist, AlternativeAudio) {
    std::unique_ptr<hls::playlist::Master_playlist> pl{parse(
      "#EXTM3U\n"
      "#EXT-X-MEDIA:TYPE=AUDIO,GROUP-ID=\"aac\",NAME=\"English\",DEFAULT=YES,"
      "AUTOSELECT=YES,LANGUAGE=\"en\", URI=\"main/english-audio.m3u8\"\n"
      "#EXT-X-MEDIA:TYPE=AUDIO,GROUP-ID=\"aac\",NAME=\"Deutsch\",DEFAULT=NO,"
      "AUTOSELECT=YES,LANGUAGE=\"de\",URI=\"main/german-audio.m3u8\"\n"
      "#EXT-X-STREAM-INF:BANDWIDTH=1280000,AUDIO=\"aac\"\n"
      "low/video-only.m3u8\n"
      "#EXT-X-STREAM-INF:BANDWIDTH=65000,CODECS=\"mp4a.40.5\",AUDIO=\"aac\"\n"
      "main/english-audio.m3u8\n"

      )};

    std::vector<const hls::playlist::IVariant_stream*> streams{pl->streams()};

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
        const hls::playlist::IRendition_group* rg{
          streams[0]->get_rendition_group(
            hls::m3u8::Media_tag::Media_type::audio)};

        ASSERT_EQ(hls::m3u8::Media_tag::Media_type::audio, rg->type());
        std::vector<const hls::m3u8::Media_tag*> media{rg->tags()};

        ASSERT_EQ(media.size(), 2);
    }
}
