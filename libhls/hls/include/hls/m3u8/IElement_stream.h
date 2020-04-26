#ifndef HLS_M3U8IELEMENTSTREAM_H_
#define HLS_M3U8IELEMENTSTREAM_H_

#include "hls/m3u8/AElement.h"

#include <memory>

namespace hls {
namespace m3u8 {

/**
 * @brief Playlist element stream
 */
class IElement_stream {
public:
    /**
     * @brief Get the next element in stream
     * @throw End_of_stream when no more elements are available
     */
    virtual std::unique_ptr<const AElement> get_next() = 0;

    virtual ~IElement_stream() = default;
};

} // namespace m3u8
} // namespace hls

#endif