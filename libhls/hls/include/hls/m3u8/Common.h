#ifndef HLS_M3U8COMMON_H_
#define HLS_M3U8COMMON_H_

#include "hls/Common.h"

#include <map>

namespace hls {
namespace m3u8 {

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