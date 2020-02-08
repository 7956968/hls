#include "Common.h"

#include "hls/Common.h"
#include "hls/m3u8/Default_parser_factory.h"

#include "hls/m3u8/Comment.h"
#include "hls/m3u8/Integer_tag.h"
#include "hls/m3u8/Tag.h"
#include "hls/m3u8/Uri.h"

class TestParser : public ::testing::Test {
public:
    static std::unique_ptr<hls::m3u8::IParser> create_parser() {
        return hls::m3u8::Default_parser_factory{}.create();
    }
};

TEST_F(TestParser, Whitespace) {
    std::unique_ptr<hls::m3u8::IParser> parser{create_parser()};

    ASSERT_THROW(parser->parse_line(""), hls::Error);
    ASSERT_THROW(parser->parse_line("    "), hls::Error);
    ASSERT_THROW(parser->parse_line("\n"), hls::Error);
    ASSERT_THROW(parser->parse_line("\r"), hls::Error);
    ASSERT_THROW(parser->parse_line("\r\n\r\n   \n"), hls::Error);
}

TEST_F(TestParser, Comment) {
    std::unique_ptr<hls::m3u8::IParser> parser{create_parser()};

    std::unique_ptr<const hls::m3u8::AElement> element{
      parser->parse_line("# comment body")};

    auto comment{dynamic_cast<const hls::m3u8::Comment*>(element.get())};
    ASSERT_TRUE(comment);
    ASSERT_EQ(" comment body", comment->body());
}

TEST_F(TestParser, EmptyComment) {
    std::unique_ptr<hls::m3u8::IParser> parser{create_parser()};

    std::unique_ptr<const hls::m3u8::AElement> element{parser->parse_line("#")};

    auto comment{dynamic_cast<const hls::m3u8::Comment*>(element.get())};
    ASSERT_TRUE(comment);
    ASSERT_TRUE(comment->body().empty());
}

TEST_F(TestParser, Uri) {
    std::unique_ptr<hls::m3u8::IParser> parser{create_parser()};

    std::unique_ptr<const hls::m3u8::AElement> element{
      parser->parse_line("https://absolute/uri/address.mp4")};

    auto uri{dynamic_cast<const hls::m3u8::Uri*>(element.get())};
    ASSERT_TRUE(uri);
    ASSERT_EQ(uri->uri(), "https://absolute/uri/address.mp4");
}

TEST_F(TestParser, M3uTag) {
    std::unique_ptr<hls::m3u8::IParser> parser{create_parser()};

    std::unique_ptr<const hls::m3u8::AElement> element{
      parser->parse_line("#EXTM3U")};

    ASSERT_EQ(element->type(), hls::m3u8::AElement::Type::tag);

    auto tag{dynamic_cast<const hls::m3u8::Tag*>(element.get())};
    ASSERT_TRUE(tag);
    ASSERT_EQ(tag->type(), hls::m3u8::Tag::Tag_type::m3u8);
}

TEST_F(TestParser, InfTag) {
    std::unique_ptr<hls::m3u8::IParser> parser{create_parser()};

    std::unique_ptr<const hls::m3u8::AElement> element{
      parser->parse_line("#EXTINF")};

    ASSERT_EQ(element->type(), hls::m3u8::AElement::Type::tag);

    auto tag{dynamic_cast<const hls::m3u8::Tag*>(element.get())};
    ASSERT_TRUE(tag);
    ASSERT_EQ(tag->type(), hls::m3u8::Tag::Tag_type::inf);
}

TEST_F(TestParser, TargetDurationTag) {
    std::unique_ptr<hls::m3u8::IParser> parser{create_parser()};

    std::unique_ptr<const hls::m3u8::AElement> element{
      parser->parse_line("#EXT-X-TARGETDURATION:42")};

    ASSERT_EQ(element->type(), hls::m3u8::AElement::Type::tag);

    auto tag{dynamic_cast<const hls::m3u8::Integer_tag*>(element.get())};
    ASSERT_TRUE(tag);
    ASSERT_EQ(tag->type(), hls::m3u8::Tag::Tag_type::x_target_duration);
    ASSERT_EQ(tag->value(), 42);
}
