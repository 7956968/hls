#ifndef HLS_M3U8MMEDIAPLAYLIST_H_
#define HLS_M3U8MMEDIAPLAYLIST_H_

#include "hls/m3u8/Inf_tag.h"
#include "hls/m3u8/Media_tag.h"
#include "hls/m3u8/Stream_inf_tag.h"
#include "hls/m3u8/Uri.h"

#include "hls/playlist/APlaylist.h"

#include <algorithm>

namespace hls {
namespace playlist {


class Media_segment {
public:
    explicit Media_segment(const std::string& uri,
                           std::shared_ptr<const m3u8::Inf_tag> inf_tag)
        : m_uri{uri}, m_inf_tag{inf_tag} {
        Expects(m_inf_tag);
    }

    const std::string& uri() const { return m_uri; }

    const m3u8::Inf_tag& inf_tag() const { return *m_inf_tag; }

private:
    std::string m_uri;

    std::shared_ptr<const m3u8::Inf_tag> m_inf_tag;
};

class Media_playlist : public APlaylist {
public:
    explicit Media_playlist(const std::string& uri) : APlaylist{uri} {}

    void add_segment(Media_segment&& segment) { m_segments.push_back(segment); }

    const std::vector<Media_segment>& media_segments() const {
        return m_segments;
    }

private:
    std::vector<Media_segment> m_segments;
};

} // namespace playlist
} // namespace hls

#endif