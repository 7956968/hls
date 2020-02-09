#include "Common.h"

#include "hls/Common.h"
#include "hls/m3u8/Comment.h"
#include "hls/m3u8/Default_parser_factory.h"
#include "hls/m3u8/Integer_tag.h"
#include "hls/m3u8/Parser_element_stream.h"
#include "hls/m3u8/String_line_reader.h"
#include "hls/m3u8/Tag.h"
#include "hls/m3u8/Uri.h"


class TestParser : public ::testing::Test {
public:
    std::unique_ptr<hls::m3u8::Parser_element_stream> create_stream(
      const std::string& source) {
        return std::make_unique<hls::m3u8::Parser_element_stream>(
          std::make_unique<hls::m3u8::String_line_reader>(source),
          hls::m3u8::Default_parser_factory{}.create());
    }
};


TEST_F(TestParser, Empty) {
    std::unique_ptr<hls::m3u8::Parser_element_stream> stream{create_stream("")};
    ASSERT_THROW(stream->get_next(), hls::End_of_stream);
}

TEST_F(TestParser, Comments) {
    std::unique_ptr<hls::m3u8::Parser_element_stream> stream{
      create_stream("#my comment\n#\n# ")};


    {
        std::unique_ptr<const hls::m3u8::AElement> element{stream->get_next()};
        auto comment{dynamic_cast<const hls::m3u8::Comment*>(element.get())};

        ASSERT_TRUE(comment);
        ASSERT_EQ(comment->body(), "my comment");
    }

    {
        std::unique_ptr<const hls::m3u8::AElement> element{stream->get_next()};
        auto comment{dynamic_cast<const hls::m3u8::Comment*>(element.get())};

        ASSERT_TRUE(comment);
        ASSERT_TRUE(comment->body().empty());
    }

    {
        std::unique_ptr<const hls::m3u8::AElement> element{stream->get_next()};
        auto comment{dynamic_cast<const hls::m3u8::Comment*>(element.get())};

        ASSERT_TRUE(comment);
        ASSERT_EQ(comment->body(), " ");
    }

    ASSERT_THROW(stream->get_next(), hls::End_of_stream);
}

TEST_F(TestParser, Whitespace) {
    std::unique_ptr<hls::m3u8::Parser_element_stream> stream{
      create_stream("\n    \n\r\n\n\r\r\n   \n")};

    ASSERT_THROW(stream->get_next(), hls::End_of_stream);
}

TEST_F(TestParser, Uri) {
    std::unique_ptr<hls::m3u8::Parser_element_stream> stream{
      create_stream("https://absolute/uri/address.mp4\n")};

    std::unique_ptr<const hls::m3u8::AElement> element{stream->get_next()};
    auto uri{dynamic_cast<const hls::m3u8::Uri*>(element.get())};

    ASSERT_TRUE(uri);
    ASSERT_EQ(uri->uri(), "https://absolute/uri/address.mp4");

    ASSERT_THROW(stream->get_next(), hls::End_of_stream);
}

TEST_F(TestParser, Tag) {
    std::unique_ptr<hls::m3u8::Parser_element_stream> stream{
      create_stream("#EXTM3U\n")};

    std::unique_ptr<const hls::m3u8::AElement> element{stream->get_next()};
    auto tag{dynamic_cast<const hls::m3u8::Tag*>(element.get())};

    ASSERT_TRUE(tag);

    ASSERT_THROW(stream->get_next(), hls::End_of_stream);
}
