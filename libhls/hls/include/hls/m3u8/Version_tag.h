#ifndef HLS_M3U8_VERSIONTAG_H_
#define HLS_M3U8_VERSIONTAG_H_

#include "hls/m3u8/Integer_tag.h"

#include "iwu/String_utils.h"


namespace hls {
namespace m3u8 {

class Version_tag : public Integer_tag {
public:
    Version_tag(const std::string& value)
        : Integer_tag{value, Tag_type::x_version} {}

    long version() const { return value(); }
};

} // namespace m3u8
} // namespace hls

#endif
