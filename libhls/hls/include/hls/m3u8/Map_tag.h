#ifndef HLS_M3U8MAPTAG_H_
#define HLS_M3U8MAPTAG_H_

#include "hls/m3u8/Attribute_list.h"
#include "hls/m3u8/Byte_range.h"
#include "hls/m3u8/Tag.h"

#include <optional.hpp>

namespace hls {
namespace m3u8 {

/**
 * @brief The EXT-X-MAP tag specifies how to obtain the Media Initialization
 * Section (Section 3) required to parse the applicable Media Segments.
 * It applies to every Media Segment that appears after it in the
 * Playlist until the next EXT-X-MAP tag or until the end of the
 * Playlist [RFC8216]
 */
class Map_tag : public Tag {
public:
    explicit Map_tag(const std::string& value);

    /**
     * @brief The value is a quoted-string containing a URI that identifies a
     * resource that contains the Media Initialization Section [RFC8216]
     */
    const std::string& uri() const;

    /**
     * @brief  Specifies a byte range into the
     * resource identified by the URI attribute. This range SHOULD
     * contain only the Media Initialization Section [RFC8216]
     */
    nonstd::optional<Byte_range> byte_range() const;

private:
    /**
     * @brief See Map_tag::uri
     */
    std::string m_uri;

    /**
     * @brief See Map_tag::byte_range
     */
    nonstd::optional<Byte_range> m_byte_range;
};

} // namespace m3u8
} // namespace hls

#endif