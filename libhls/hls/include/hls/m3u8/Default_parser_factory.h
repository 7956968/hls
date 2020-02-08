#ifndef HLS_M3U8_DEFAULTPARSERFACTORY_H_
#define HLS_M3U8_DEFAULTPARSERFACTORY_H_

#include "hls/m3u8/IParser_factory.h"
// #include "hls/m3u8/Tag.h"

// #include "hls/m3u8/Media_tag.h"
// #include "hls/m3u8/Parser.h"
// #include "hls/m3u8/Stream_inf_tag.h"
// #include "hls/m3u8/Version_tag.h"

namespace hls {
namespace m3u8 {

class Default_parser_factory : public IParser_factory {
public:
    std::unique_ptr<IParser> create() override;
};

} // namespace m3u8
} // namespace hls

#endif