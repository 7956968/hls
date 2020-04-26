#include "Mock_repository.h"

#include "hls/Common.h"
#include "hls/m3u8/Comment.h"
#include "hls/m3u8/Default_parser_factory.h"
#include "hls/m3u8/Integer_tag.h"
#include "hls/m3u8/Key_tag.h"
#include "hls/m3u8/Parser_element_stream.h"
#include "hls/m3u8/String_line_reader.h"
#include "hls/m3u8/Tag.h"
#include "hls/m3u8/Uri.h"
#include "hls/m3u8/Variable_resolver.h"

#include "iwu/test/Test_common.h"

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

TEST_F(TestParser, EnumTag) {
    std::unique_ptr<hls::m3u8::Parser_element_stream> stream{
      create_stream("#EXT-X-PLAYLIST-TYPE:EVENT")};

    std::unique_ptr<const hls::m3u8::AElement> element{stream->get_next()};
    auto tag{dynamic_cast<const hls::m3u8::Tag*>(element.get())};

    ASSERT_TRUE(tag);


    ASSERT_THROW(stream->get_next(), hls::End_of_stream);
}

TEST_F(TestParser, Variables) {
    Mock_repository repo;
    repo.m_imports["imported"] = "imported_value";

    hls::m3u8::Variable_resolver resolver{&repo};

    auto parser{hls::m3u8::Default_parser_factory{}.create()};

    parser->set_variable_repository(&repo);
    parser->set_variable_resolver(&resolver);

    // Variables only work on URIs and quoted strings in attribute lists, so
    // check those two only
    auto stream{std::make_unique<hls::m3u8::Parser_element_stream>(
      std::make_unique<hls::m3u8::String_line_reader>(
        "#EXT-X-DEFINE:NAME=\"var1\",VALUE=\"value1\"\n"
        "#EXT-X-DEFINE:NAME=\"var2\",VALUE=\"value2\"\n"
        "#EXT-X-DEFINE:IMPORT=\"imported\"\n"
        "http://{$var1}_{$var2}_{$imported}\n"
        "#EXT-X-KEY:URI=\"first {$var1} second {$imported}\",METHOD=AES-128\n"),
      std::move(parser))};

    std::vector<std::unique_ptr<const hls::m3u8::AElement>> elements;

    while (true) {
        try {
            elements.push_back(stream->get_next());
        } catch (const hls::End_of_stream&) { break; }
    }

    ASSERT_EQ(repo.m_variables["var1"], "value1"s);
    ASSERT_EQ(repo.m_variables["var2"], "value2"s);
    ASSERT_EQ(repo.m_variables["imported"], "imported_value"s);

    ASSERT_EQ(dynamic_cast<const hls::m3u8::Uri*>(elements[3].get())->uri(),
              "http://value1_value2_imported_value"s);
    ASSERT_EQ(
      dynamic_cast<const hls::m3u8::Key_tag*>(elements[4].get())->uri().value(),
      "first value1 second imported_value"s);
}
