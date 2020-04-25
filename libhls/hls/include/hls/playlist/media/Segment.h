#ifndef HLS_PLAYLISTMEDIASEGMENT_H_
#define HLS_PLAYLISTMEDIASEGMENT_H_

#include "hls/m3u8/Inf_tag.h"

namespace hls {
namespace playlist {
namespace media {

/**
 * @brief Media segment
 */
class Segment {
public:
    /**
     * @brief Constructor
     *
     * @param uri See Segment::uri
     * @param inf_tag See Segment::inf_tag
     */
    explicit Segment(const std::string& uri,
                     std::shared_ptr<const m3u8::Inf_tag> inf_tag)
        : m_uri{uri}, m_inf_tag{inf_tag} {
        Expects(m_inf_tag);
    }

    /**
     * @brief URI which can be used to download the segment
     */
    const std::string& uri() const { return m_uri; }

    /**
     * @brief Accompanying segment info
     */
    const m3u8::Inf_tag& inf_tag() const { return *m_inf_tag; }

private:
    /**
     * @brief See Segment::uri
     */
    std::string m_uri;

    /**
     * @brief See Segment::inf_tag
     */
    std::shared_ptr<const m3u8::Inf_tag> m_inf_tag;
};

} // namespace media
} // namespace playlist
} // namespace hls

#endif