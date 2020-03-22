#include "hls/m3u8/Default_parser_factory.h"
#include "hls/m3u8/Parser_element_stream.h"
#include "hls/m3u8/String_line_reader.h"
#include "hls/playlist/Media_playlist_parser.h"

#include "iwu/test/Test_common.h"

class TestMediaPlaylist : public ::testing::Test {
public:
    std::unique_ptr<hls::m3u8::Parser_element_stream> create_stream(
      const std::string& source) {
        return std::make_unique<hls::m3u8::Parser_element_stream>(
          std::make_unique<hls::m3u8::String_line_reader>(source),
          hls::m3u8::Default_parser_factory{}.create());
    }

    std::unique_ptr<hls::playlist::Media_playlist> parse(
      const std::string& input) {
        auto stream{create_stream(input)};

        return hls::playlist::Media_playlist_parser{stream.get()}.parse(""s);
    }
};


TEST_F(TestMediaPlaylist, Main) {
    std::unique_ptr<hls::playlist::Media_playlist> pl{
      parse("#EXTM3U\n"
            "#EXT-X-TARGETDURATION:10\n"
            "#EXT-X-VERSION:3\n"
            "#EXTINF:9.009,\n"
            "http://media.example.com/first.ts\n"
            "#EXTINF:9.009,\n"
            "http://media.example.com/second.ts\n"
            "#EXTINF:3.003,\n"
            "http://media.example.com/third.ts\n"
            "#EXT-X-ENDLIST\n")};

    const std::vector<hls::playlist::Media_segment>& segments{
      pl->media_segments()};
    ASSERT_EQ(3, segments.size());

    ASSERT_EQ(segments[0].uri(), "http://media.example.com/first.ts");
    ASSERT_EQ(segments[1].uri(), "http://media.example.com/second.ts");
    ASSERT_EQ(segments[2].uri(), "http://media.example.com/third.ts");
};