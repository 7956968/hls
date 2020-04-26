#ifndef HLS_PLAYLISTIRENDITIONGROUP_H_
#define HLS_PLAYLISTIRENDITIONGROUP_H_

#include "hls/m3u8/Media_tag.h"
#include "hls/m3u8/Uri.h"

namespace hls {
namespace playlist {
namespace master {

/**
 * @brief A set of one or more EXT-X-MEDIA tags with the same GROUP-ID value
 * and the same TYPE value defines a Group of Renditions
 * Each member of the Group MUST be an alternative Rendition of the same
 * content; otherwise, playback errors can occur [RFC8216]
 */
class IRendition_group {
public:
    /**
     * @brief Media type of this group (e.g. audio, video, etc.)
     */
    virtual m3u8::Media_tag::Media_type type() const = 0;

    /**
     * @brief Group ID, all the media playlists of the same group share it
     */
    virtual const std::string& id() const = 0;

    /**
     * @brief Media tags (one tag = one playlist)
     */
    virtual std::vector<const m3u8::Media_tag*> tags() const = 0;

    /**
     * @brief Desctructor
     */
    virtual ~IRendition_group() = default;
};

} // namespace master
} // namespace playlist
} // namespace hls

#endif
