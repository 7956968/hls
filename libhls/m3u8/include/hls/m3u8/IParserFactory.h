#ifndef HLS_M3U8_IPARSERFACTORY_H_
#define HLS_M3U8_IPARSERFACTORY_H_

#include <hls/m3u8/IParser.h>

namespace hls {

namespace m3u8 {


class IParserFactory {
public:
    virtual IParser* create() = 0;

    virtual ~IParserFactory() {
    }
};

} // m3u8

} // hls

#endif