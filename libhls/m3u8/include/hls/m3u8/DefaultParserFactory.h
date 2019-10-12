#ifndef HLS_M3U8_DEFAULTPARSERFACTORY_H_
#define HLS_M3U8_DEFAULTPARSERFACTORY_H_

#include <hls/m3u8/IParserFactory.h>
#include <hls/m3u8/Tag.h>

#include <hls/m3u8/Parser.h>

#include <hls/m3u8/StreamInfTag.h>
#include <hls/m3u8/VersionTag.h>
#include <hls/m3u8/MediaTag.h>

namespace hls {

namespace m3u8 {

class DefaultParserFactory : public IParserFactory {
public:

    IParser* create() override {
        Parser* parser = new Parser();

         // Register tags
        parser
            ->registerTagType("M3U", Tag::TagType::M3U)
            .registerTagType("INF", Tag::TagType::INF)
            .registerTagType("-X-ENDLIST", Tag::TagType::X_END_LIST)
            .registerIntegerTagType("-X-VERSION", Tag::TagType::X_VERSION)
            .registerIntegerTagType("-X-MEDIA-SEQUENCE", Tag::TagType::X_MEDIA_SEQUENCE)
            .registerIntegerTagType("-X-TARGETDURATION", Tag::TagType::X_TARGET_DURATION)
            .registerParsedTagType<MediaTag>("-X-MEDIA", Tag::TagType::X_MEDIA)
            .registerParsedTagType<StreamInfTag>("-X-STREAM-INF", Tag::TagType::X_STREAM_INF)
        ;

        return parser;
    }
};

} // m3u8

} // hls

#endif