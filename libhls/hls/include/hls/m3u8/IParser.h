#ifndef HLS_M3U8_IPARSER_H_
#define HLS_M3U8_IPARSER_H_

#include "hls/m3u8/AElement.h"

#include <string>

namespace hls {
namespace m3u8 {

class IParser {
public:
    virtual const AElement* parse_line(const std::string& line) = 0;

    virtual ~IParser() = default;
};

} // namespace m3u8
} // namespace hls

#endif