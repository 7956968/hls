#ifndef HLS_M3U8_STARTTAG_H_
#define HLS_M3U8_STARTTAG_H_

#include "hls/m3u8/Attribute_list.h"
#include "hls/m3u8/Tag.h"

#include <optional.hpp>

namespace hls {
namespace m3u8 {

/**
 * @brief The EXT-X-START tag indicates a preferred point at which to start
 * playing a Playlist. By default, clients SHOULD start playback at
 * this point when beginning a playback session. This tag is OPTIONAL [RFC8216b]
 */
class Start_tag : public Tag {
public:
public:
    /**
     * @brief Constructor
     */
    explicit Start_tag(const std::string& value,
                       const IVariable_resolver* variable_resolver);

    /**
     * @brief The value of TIME-OFFSET is a signed-decimal-floating-point number
     * of seconds. A positive number indicates a time offset from the
     * beginning of the Playlist. A negative number indicates a negative
     * time offset from the end of the last Media Segment in the
     * Playlist. This attribute is REQUIRED [RFC8216b]
     */
    float time_offset() const;

    /**
     * The value is an enumerated-string; valid strings are YES and NO.
     * If the value is YES, clients SHOULD start playback at the Media
     * Segment containing the TIME-OFFSET, but SHOULD NOT render media
     * samples in that segment whose presentation times are prior to the
     * TIME-OFFSET.
     * If the value is NO, clients SHOULD attempt to render
     * every media sample in that segment.
     * This attribute is OPTIONAL.
     * If it is missing, its value should be treated as NO. [RFC8216b]
     */
    bool precise() const;

private:
    /**
     * @brief See Start_tag::time_offset
     */
    float m_time_offset;

    /**
     * @brief See Start_tag::precise
     */
    bool m_precise{false};
};

} // namespace m3u8
} // namespace hls

#endif