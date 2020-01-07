#ifndef HLS_M3U8_IPARSERFACTORY_H_
#define HLS_M3U8_IPARSERFACTORY_H_

#include "hls/m3u8/IParser.h"

#include <memory>

namespace hls {
namespace m3u8 {

class IParser_factory {
public:
    virtual std::unique_ptr<IParser> create() = 0;

    virtual ~IParser_factory() = default;
};

} // namespace m3u8
} // namespace hls

#endif