#ifndef HLS_M3U8COMMON_H_
#define HLS_M3U8COMMON_H_

#include "hls/Common.h"

#include <map>

namespace hls {
namespace m3u8 {

/**
 * @brief Playlits type as defined in RFC8216b/4.4.3.5 EXT-X-PLAYLIST-TYPE
 */
enum class Playlist_type {
    /**
     * @brief Indicates that Media Segments can only be added to the end of the
     * Media Playlist
     */
    event,

    /**
     * @brief Indicates the Media Playlist cannot change
     */
    vod
};

/**
 * @brief Enum parser string
 *
 * @tparam T Enum class
 */
template<typename T>
using Enum_parser_fnc = std::function<T(const std::string&)>;

/**
 * @brief Get the playlist type enum parser
 */
const Enum_parser_fnc<Playlist_type>& get_playlist_type_parser();

/**
 * @brief Parse an enum string
 *
 * @tparam T enum class
 * @param name_map Name/value map
 * @param name Field name
 */
template<typename T>
T parse_enum_string(const std::map<std::string, T>& name_map,
                    const std::string& name) {
    auto name_iter{name_map.find(name)};
    Expects(name_iter != name_map.cend(),
            hls::Error{"Unexpected enum name: '"s + name + "'"s});

    return name_iter->second;
};

} // namespace m3u8
} // namespace hls

#endif