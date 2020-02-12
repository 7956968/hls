#ifndef HLS_M3U8MMEDIAPLAYLIST_H_
#define HLS_M3U8MMEDIAPLAYLIST_H_

#include "hls/m3u8/Media_tag.h"
#include "hls/m3u8/Stream_inf_tag.h"
#include "hls/m3u8/Uri.h"


#include <algorithm>

namespace hls {
namespace playlist {


class Media_segment {
public:
    explicit Media_segment(const std::string& uri) : m_uri{uri} {}

    const std::string& uri() const { return m_uri; }

private:
    std::string m_uri;
};

class Media_playlist {
public:
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