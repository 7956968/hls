#include "hls/m3u8/Common.h"


namespace hls {
namespace m3u8 {

namespace {

const Enum_parser_fnc<Playlist_type> s_playlist_type_parser{
  [](const std::string& name) {
      return hls::m3u8::parse_enum_string(
        std::map<std::string, Playlist_type>{{"EVENT", Playlist_type::event},
                                             {"VOD", Playlist_type::vod}},
        name);
  }};

}
const Enum_parser_fnc<Playlist_type>& get_playlist_type_parser() {
    return s_playlist_type_parser;
}

} // namespace m3u8

} // namespace hls