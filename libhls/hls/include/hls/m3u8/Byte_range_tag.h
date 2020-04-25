#ifndef HLS_M3U8BYTERANGETAG_H
#define HLS_M3U8BYTERANGETAG_H

#include "hls/m3u8/Attribute_list.h"
#include "hls/m3u8/Byte_range.h"
#include "hls/m3u8/Tag.h"

#include <optional.hpp>

namespace hls {
namespace m3u8 {

/**
 * @brief Tag indicates that a Media Segment is a sub-range
 * of the resource identified by its URI.
 *
 * It applies only to the next URI line that follows it in the Playlist.
 * [RFC8216]
 */
class Byte_range_tag : public Tag {
public:
    Byte_range_tag(const std::string& value)
        : Tag{Tag_type::x_byte_range}, m_range{value} {}

    const Byte_range& byte_range() const { return m_range; }

private:
    Byte_range m_range;
};

} // namespace m3u8
} // namespace hls

#endif
