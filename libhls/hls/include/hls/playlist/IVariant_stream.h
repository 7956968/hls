#ifndef HLS_PLAYLISTIVARIANTSTREAM_H_
#define HLS_PLAYLISTIVARIANTSTREAM_H_

#include "hls/m3u8/Stream_inf_tag.h"
#include "hls/m3u8/Uri.h"
#include "hls/playlist/IRendition_group.h"

namespace hls {
namespace playlist {

class IVariant_stream {
public:
    virtual ~IVariant_stream() = default;

    virtual const m3u8::Uri& uri() const = 0;

    virtual const m3u8::Stream_inf_tag& stream_inf() const = 0;

    virtual const IRendition_group* get_rendition_group(
      m3u8::Media_tag::Media_type type) const = 0;
};

} // namespace playlist
} // namespace hls
#endif