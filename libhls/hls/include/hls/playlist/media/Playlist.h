#ifndef HLS_M3U8MMEDIAPLAYLIST_H_
#define HLS_M3U8MMEDIAPLAYLIST_H_

#include "hls/m3u8/Inf_tag.h"
#include "hls/m3u8/Media_tag.h"
#include "hls/m3u8/Stream_inf_tag.h"
#include "hls/m3u8/Uri.h"

#include "hls/m3u8/Common.h"
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
    explicit Playlist(const std::string& uri,
                      std::chrono::nanoseconds target_duration)
        : APlaylist{uri}, m_target_duration{target_duration} {}

    /**
     * @brief List of media segments
     */
    const std::vector<Segment>& segments() const { return m_segments; }

    /**
     * @brief Indication that each segment in the playlist describes a single
     * I-frame
     */
    bool is_iframes_only() const { return m_iframes_only; }

    /**
     * @brief Playlist type
     */
    nonstd::optional<m3u8::Playlist_type> type() const { return m_type; }

    /**
     * @brief Updper bound on the duration of all media segments in this
     * playlist
     */
    std::chrono::nanoseconds target_duration() const {
        return m_target_duration;
    }

    Playlist& set_segments(const std::vector<Segment>& segments) {
        m_segments = segments;
        return *this;
    }

    Playlist& set_type(m3u8::Playlist_type type) {
        m_type = type;
        return *this;
    }

    Playlist& set_iframes_only(bool iframes_only) {
        m_iframes_only = iframes_only;
        return *this;
    }

private:
    /**
     * @brief See Media_playlist::target_duration
     */
    std::chrono::nanoseconds m_target_duration;

    /**
     * @brief See Media_playlist::segments
     */
    std::vector<Segment> m_segments;

    /**
     * @brief See Media_playlist::type
     */
    nonstd::optional<m3u8::Playlist_type> m_type;

    /**
     * @brief See Media_playlist::is_iframes_only
     */
    bool m_iframes_only{false};
};

} // namespace media
} // namespace playlist
} // namespace hls

#endif