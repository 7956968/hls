#ifndef HLS_PLAYLISTAPLAYLIST_H_
#define HLS_PLAYLISTAPLAYLIST_H_

#include <string>

namespace hls {
namespace playlist {

class APlaylist {
public:
    explicit APlaylist(const std::string& uri) : m_uri{uri} {
        const size_t file_pos{uri.rfind('/')};
        if (file_pos != std::string::npos) {
            m_uri = uri.substr(0, file_pos);
        }
    }

    const std::string& base_uri() const { return m_uri; }

private:
    std::string m_uri;
};

} // namespace playlist
} // namespace hls

#endif