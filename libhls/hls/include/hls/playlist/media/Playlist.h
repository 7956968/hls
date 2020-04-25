#ifndef HLS_M3U8MMEDIAPLAYLIST_H_
#define HLS_M3U8MMEDIAPLAYLIST_H_

#include "hls/m3u8/Inf_tag.h"
#include "hls/m3u8/Media_tag.h"
#include "hls/m3u8/Stream_inf_tag.h"
#include "hls/m3u8/Uri.h"

#include "hls/playlist/APlaylist.h"
#include "hls/playlist/media/Segment.h"

#include <algorithm>

namespace hls {
namespace playlist {
namespace media {

/**
 * @brief Playlist which contains a list of media segments. When they're played
 * sequentially, they make up a multimedia presentation.
 *
 * A Playlist is a Media Playlist if all URI lines in the Playlist identify
 * Media Segment
 *
 * A Playlist MUST be either a Media Playlist or a Master
 * Playlist; all other Playlists are invalid (source rfc8216)
 */
class Playlist : public APlaylist {
public:
    /**
     * @brief Constructor
     *
     * @param uri Playlist base URI
     */
    explicit Playlist(const std::string& uri) : APlaylist{uri} {}

    /**
     * @brief Add a new segment to the list
     * TODO Make protected
     *
     * @param segment Media segment
     */
    void add_segment(Segment&& segment) { m_segments.push_back(segment); }

    /**
     * @brief List of segments
     */
    const std::vector<Segment>& segments() const { return m_segments; }

private:
    /**
     * @brief See Media_playlist::segments
     */
    std::vector<Segment> m_segments;
};

} // namespace media
} // namespace playlist
} // namespace hls

#endif