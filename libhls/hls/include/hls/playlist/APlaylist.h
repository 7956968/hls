#ifndef HLS_PLAYLISTAPLAYLIST_H_
#define HLS_PLAYLISTAPLAYLIST_H_

#include "hls/Common.h"

#include <string>

namespace hls {
namespace playlist {

/**
 * @brief Base class for playlists
 */
class APlaylist {
public:
    explicit APlaylist(const std::string& uri) : m_uri{uri} {
        const size_t file_pos{uri.rfind('/')};
        if (file_pos != std::string::npos) {
            m_uri = uri.substr(0, file_pos);
        }
    }

    /**
     * @brief Playlist base URI
     */
    const std::string& base_uri() const { return m_uri; }

    /**
     * @brief Indication that all media samples can be decoded without
     * information from other segments
     */
    bool independent_segments() const { return m_independent_segments; }

    /**
     * @brief Indicates a preferred point at which to start playing this
     * playlist
     * A positive value indicates a time offset from the beginning of the
     * playlist A negative number indicates a negative offset from the end of
     * the last media segment in the playlist
     *
     * @return nonstd::optional<std::chrono::nanoseconds>
     */
    nonstd::optional<std::chrono::nanoseconds> time_offset() const {
        return m_time_offset;
    }

    /**
     * @brief If true, clients SHOULD start playback at the Media Segment
     * containing the ::time_offset, but SHOULD NOT render media samples in that
     * segment whose presentation times are prior to the TIME-OFFSET. If false,
     * clients SHOULD attempt to render every media sample in that segment
     */
    bool precise_offset() const { return m_precise_offset; }

    APlaylist& set_independent_segments(bool independent_segments) {
        m_independent_segments = independent_segments;

        return *this;
    }

    APlaylist& set_time_offset(std::chrono::nanoseconds time_offset) {
        m_time_offset = time_offset;
        return *this;
    }

    APlaylist& set_precise_offset(bool precise_offset) {
        m_precise_offset = precise_offset;
        return *this;
    }

private:
    /**
     * @brief See APlaylist::base_uri
     */
    std::string m_uri;

    /**
     * @brief See APlaylist::independent_segments
     */
    bool m_independent_segments{false};

    /**
     * @brief See APlaylist::time_offset
     */
    nonstd::optional<std::chrono::nanoseconds> m_time_offset;

    /**
     * @brief See APlaylist::precise_offset
     */
    bool m_precise_offset{false};
};

} // namespace playlist
} // namespace hls

#endif