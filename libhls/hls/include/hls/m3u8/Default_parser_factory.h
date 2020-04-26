#ifndef HLS_M3U8_DEFAULTPARSERFACTORY_H_
#define HLS_M3U8_DEFAULTPARSERFACTORY_H_

#include "hls/m3u8/IParser_factory.h"

namespace hls {
namespace m3u8 {

/**
 * @brief Default element parser factory
 */
class Default_parser_factory : public IParser_factory {
    /**
     * @brief IParser_factory implementation
     */
public:
    std::unique_ptr<IParser> create() override;
};

} // namespace m3u8
} // namespace hls

#endif