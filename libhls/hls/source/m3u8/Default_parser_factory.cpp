#include "hls/m3u8/Default_parser_factory.h"
#include "hls/m3u8/Parser.h"

namespace hls {

namespace m3u8 {

std::unique_ptr<IParser> Default_parser_factory::create() {
    return std::make_unique<Parser>();
}

} // namespace m3u8
} // namespace hls