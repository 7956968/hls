#ifndef HLS_M3U8INFTAG_H_
#define HLS_M3U8INFTAG_H_

#include "hls/m3u8/Tag.h"

namespace hls {
namespace m3u8 {

/**
 * @brief The EXTINF tag specifies the duration of a Media Segment. It applies
 * only to the next Media Segment. This tag is REQUIRED for each Media
 * Segment [RFC8216]
 */
class Inf_tag : public Tag {
public:
    explicit Inf_tag(const std::string& value);

    /**
     * @brief Durations SHOULD be decimal-floating-
     * point, with enough accuracy to avoid perceptible error when segment
     * durations are accumulated. However, if the compatibility version
     * number is less than 3, durations MUST be integers. Durations that
     * are reported as integers SHOULD be rounded to the nearest integer
     * [RFC8216]
     */
    double duration() const;

    /**
     * @brief Optional human readable informative title of the Media Segment
     * [RFC8216]
     */
    nonstd::optional<std::string> title() const;

private:
    /**
     * @brief See Inf_tag::duration
     */
    double m_duration{-1};

    /**
     * @brief See Inf_Tag::title
     */
    nonstd::optional<std::string> m_title;
};

} // namespace m3u8
} // namespace hls

#endif