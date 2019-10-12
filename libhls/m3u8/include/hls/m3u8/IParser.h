#ifndef HLS_M3U8_IPARSER_H_
#define HLS_M3U8_IPARSER_H_

#include <hls/m3u8/AElement.h>

#include <string>

namespace hls {

namespace m3u8 {

class IParser {
public:
    virtual const AElement* parseLine(const std::string& line) = 0;

    virtual ~IParser() {
    }
};

} // m3u8

} // hls

#endif