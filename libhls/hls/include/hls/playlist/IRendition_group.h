#ifndef HLS_PLAYLISTIRENDITIONGROUP_H_
#define HLS_PLAYLISTIRENDITIONGROUP_H_

#include "hls/m3u8/Media_tag.h"
#include "hls/m3u8/Uri.h"

namespace hls {
namespace playlist {

class IRendition_group {
public:
    virtual ~IRendition_group() = default;

    virtual m3u8::Media_tag::Media_type type() const = 0;

    virtual const std::string& id() const = 0;

    virtual std::vector<const m3u8::Media_tag*> tags() const = 0;
};

} // namespace playlist
} // namespace hls

#endif
