#include "ATest.h"

#include <hls/m3u8/DefaultParserFactory.h>

#include <hls/m3u8/IntegerTag.h>

class TestParser: public ATest {
public:
    virtual ~TestParser() {
    }

    std::unique_ptr<hls::m3u8::IParser> createParser() {
        return std::unique_ptr<hls::m3u8::IParser>(mFactory.create());
    }

    hls::m3u8::DefaultParserFactory mFactory;
};

GROUP_TEST_F(Parser, TestParser, Whitespace) {
    auto parser = createParser();

    ASSERT_EQ(parser->parseLine(""), nullptr);
    ASSERT_EQ(parser->parseLine("    "), nullptr);
    ASSERT_EQ(parser->parseLine("\n"), nullptr);
    ASSERT_EQ(parser->parseLine("\r"), nullptr);
    ASSERT_EQ(parser->parseLine("\r\n\r\n   \n"), nullptr);
}

GROUP_TEST_F(Parser, TestParser, Comment) {
    auto parser = createParser();

    auto element = std::unique_ptr<const hls::m3u8::AElement>();

    // Regular comment
    element.reset( parser->parseLine("# comment body") );
    ASSERT_TRUE(element != nullptr);
    ASSERT_EQ(element->getType(), hls::m3u8::AElement::Type::COMMENT);
    ASSERT_EQ(dynamic_cast<const hls::m3u8::Comment*>(element.get())->getBody(),
        " comment body");

    // Empty comment
    element.reset( parser->parseLine("#") );
    ASSERT_TRUE(element != nullptr);
    ASSERT_EQ(element->getType(), hls::m3u8::AElement::Type::COMMENT);

    ASSERT_EQ(dynamic_cast<const hls::m3u8::Comment*>(element.get())->getBody(),
        "");
}

GROUP_TEST_F(Parser, TestParser, Uri) {
    auto parser = createParser();

    auto element = std::unique_ptr<const hls::m3u8::AElement>();

    // Relative URI
    element.reset( parser->parseLine("relative/uri/address.mp4") );
    ASSERT_TRUE(element != nullptr);
    ASSERT_EQ(element->getType(), hls::m3u8::AElement::Type::URI);
        ASSERT_EQ(dynamic_cast<const hls::m3u8::Uri*>(element.get())->getUri(),
        "relative/uri/address.mp4");

    // Absolute URI
    element.reset( parser->parseLine("https://absolute/uri/address.mp4") );
    ASSERT_TRUE(element != nullptr);
    ASSERT_EQ(element->getType(), hls::m3u8::AElement::Type::URI);
        ASSERT_EQ(dynamic_cast<const hls::m3u8::Uri*>(element.get())->getUri(),
        "https://absolute/uri/address.mp4");
}

GROUP_TEST_F(Parser, TestParser, M3uTag) {
    auto parser = createParser();

    auto element = std::unique_ptr<const hls::m3u8::AElement>();

    // Relative URI
    element.reset( parser->parseLine("#EXTM3U") );
    ASSERT_TRUE(element != nullptr);
    ASSERT_EQ(element->getType(), hls::m3u8::AElement::Type::TAG);
        ASSERT_EQ(dynamic_cast<const hls::m3u8::Tag*>(element.get())->getType(),
        hls::m3u8::Tag::TagType::M3U);
}

GROUP_TEST_F(Parser, TestParser, InfTag) {
    auto parser = createParser();

    auto element = std::unique_ptr<const hls::m3u8::AElement>();

    // Relative URI
    element.reset( parser->parseLine("#EXTINF") );
    ASSERT_TRUE(element != nullptr);
    ASSERT_EQ(element->getType(), hls::m3u8::AElement::Type::TAG);
        ASSERT_EQ(dynamic_cast<const hls::m3u8::Tag*>(element.get())->getType(),
        hls::m3u8::Tag::TagType::INF);
}

GROUP_TEST_F(Parser, TestParser, AttributeList) {
    hls::m3u8::AttributeList al;

    ASSERT_EQ(hls::Status::OK, al.fromString(
        "string_key=\"test string value\""
        ",decimal_key=42,float_key=4.2,bool_key=YES"
        ",resolution_key=120x320,enum_key=ENUM_VALUE"
    ));

    ASSERT_EQ(al.getAttributes().size(), 6U);

    std::string strValue;
    EXPECT_TRUE(al.getString("string_key", &strValue));
    EXPECT_EQ(strValue, "test string value");

    int64_t decValue;
    EXPECT_TRUE(al.getInteger("decimal_key", &decValue));
    EXPECT_EQ(decValue, 42);

    float floatValue;
    EXPECT_TRUE(al.getFloat("float_key", &floatValue));
    EXPECT_FLOAT_EQ(floatValue, 4.2);

    bool boolValue;
    EXPECT_TRUE(al.getBoolEnum("bool_key", &boolValue));
    EXPECT_EQ(boolValue, true);

    std::pair<int32_t, int32_t> resolution;
    EXPECT_TRUE(al.getResolution("resolution_key", &resolution));
    EXPECT_EQ(resolution.first, 120);
    EXPECT_EQ(resolution.second, 320);

    EXPECT_TRUE(al.getEnumString("enum_key", &strValue));
    EXPECT_EQ(strValue, "ENUM_VALUE");


}

GROUP_TEST_F(Parser, TestParser, TargetDuration) {
    auto parser = createParser();

    auto element = std::unique_ptr<const hls::m3u8::AElement>();

    {
        // Correct value
        element.reset( parser->parseLine("#EXT-X-TARGETDURATION:42") );
        ASSERT_TRUE(element != nullptr);
        ASSERT_EQ(element->getType(), hls::m3u8::AElement::Type::TAG);
            ASSERT_EQ(dynamic_cast<const hls::m3u8::Tag*>(element.get())->getType(),
            hls::m3u8::Tag::TagType::X_TARGET_DURATION);

        ASSERT_EQ(element->getType(), hls::m3u8::AElement::Type::TAG);
            ASSERT_EQ(dynamic_cast<const hls::m3u8::IntegerTag*>(element.get())->getValue(),
                42);
    }

    {
        // No value
        element.reset( parser->parseLine("#EXT-X-TARGETDURATION:") );
        ASSERT_TRUE(element == nullptr);
    }

    {
        // Invalid value
        element.reset( parser->parseLine("#EXT-X-TARGETDURATION:asdf") );
        ASSERT_TRUE(element == nullptr);
    }

    {
        // Invalid value
        element.reset( parser->parseLine("#EXT-X-TARGETDURATION:42.3") );
        ASSERT_TRUE(element == nullptr);
    }

    {
        // Invalid value
        element.reset( parser->parseLine("#EXT-X-TARGETDURATION:0x42") );
        ASSERT_TRUE(element == nullptr);
    }
}