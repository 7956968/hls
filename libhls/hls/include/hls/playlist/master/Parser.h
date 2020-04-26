#ifndef HLS_PLAYLISTMASTERPLAYLISTPARSER_H_
#define HLS_PLAYLISTMASTERPLAYLISTPARSER_H_

#include "hls/Common.h"
#include "hls/m3u8/IElement_stream.h"
#include "hls/playlist/APlaylist_parser.h"
#include "hls/playlist/master/Playlist.h"

#include <vector>

namespace hls {
namespace playlist {
namespace master {

/**
 * @brief Master playlist parser
 */
class Parser : public APlaylist_parser {
public:
    /**
     * @brief Parse a playlist
     *
     * @param stream Element stream
     * @param base_uri Playlist base uri
     */
    static std::unique_ptr<Playlist> parse(m3u8::IElement_stream* stream,
                                           const std::string& base_uri);

private:
    /**
     * @brief Constructor
     *
     * @param stream Element stream
     * @param base_uri Playlist base uri
     */
    Parser(m3u8::IElement_stream* stream, const std::string& base_uri);

    /**
     * @brief Parse the playlist
     */
    std::unique_ptr<Playlist> parse();

private:
    /**
     * @brief Playlist base URI
     */
    std::string m_base_uri;
};

} // namespace master
} // namespace playlist
} // namespace hls

#endif
