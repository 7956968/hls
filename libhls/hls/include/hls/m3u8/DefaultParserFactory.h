#ifndef HLS_M3U8_DEFAULTPARSERFACTORY_H_
#define HLS_M3U8_DEFAULTPARSERFACTORY_H_

#include "hls/m3u8/IParser_factory.h"
#include "hls/m3u8/Tag.h"

#include "hls/m3u8/Media_tag.h"
#include "hls/m3u8/Parser.h"
#include "hls/m3u8/Stream_inf_tag.h"
#include "hls/m3u8/Version_tag.h"

namespace hls {
namespace m3u8 {

class Default_parser_factory : public IParser_factory {
public:
    std::unique_ptr<IParser> create() override {
        auto parser{std::make_unique<Parser>()};

        // Register tags
        parser->registerTagType("M3U", Tag::Tag_type::M3U)
          .registerTagType("INF", Tag::Tag_type::INF)
          .registerTagType("-X-ENDLIST", Tag::Tag_type::X_END_LIST)
          .registerIntegerTagType("-X-VERSION", Tag::Tag_type::X_VERSION)
          .registerIntegerTagType("-X-MEDIA-SEQUENCE",
                                  Tag::Tag_type::X_MEDIA_SEQUENCE)
          .registerIntegerTagType("-X-TARGETDURATION",
                                  Tag::Tag_type::X_TARGET_DURATION)
          .registerParsedTagType<MediaTag>("-X-MEDIA", Tag::Tag_type::X_MEDIA)
          .registerParsedTagType<StreamInfTag>("-X-STREAM-INF",
                                               Tag::Tag_type::X_STREAM_INF);

        return parser;
    }
};

} // namespace m3u8
} // namespace hls

#endif