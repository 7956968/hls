#ifndef HLS_PLAYLISTIVARIANTSTREAM_H_
#define HLS_PLAYLISTIVARIANTSTREAM_H_

#include "hls/m3u8/Stream_inf_tag.h"
#include "hls/m3u8/Uri.h"
#include "hls/playlist/master/IRendition_group.h"

namespace hls {
namespace playlist {
namespace master {

/**
 * @brief A logical set of renditions which can be combined to play a
 * representation
 */
class IVariant_stream {
public:
    /**
     * @brief The URI line that follows the EXT-X-STREAM-INF tag (specifies a
     * Media Playlist that carries a Rendition of the Variant Stream)
     */
    virtual const m3u8::Uri& uri() const = 0;

    /**
     * @brief Parent Stream_inf
     */
    virtual const m3u8::Stream_inf_tag& stream_inf() const = 0;

    /**
     * @brief Get the rendition group with given type
     *
     * @param type Media type
     * @return Rendition Group
     */
    virtual const IRendition_group* get_rendition_group(
      m3u8::Media_tag::Media_type type) const = 0;

    /**
     * @brief Destructor
     */
    virtual ~IVariant_stream() = default;
};

} // namespace master
} // namespace playlist
} // namespace hls

#endif