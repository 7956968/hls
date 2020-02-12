#ifndef HLS_M3U8COMMON_H_
#define HLS_M3U8COMMON_H_

#include "hls/Common.h"

#include <map>

namespace hls {
namespace m3u8 {


enum class Playlist_type { event, vod };


template<typename T>
using Enum_parser_fnc = std::function<T(const std::string&)>;

const Enum_parser_fnc<Playlist_type>& get_playlist_type_parser();

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