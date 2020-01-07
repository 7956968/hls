#ifndef HLS_M3U8_TAG_H_
#define HLS_M3U8_TAG_H_

#include "hls/m3u8/AElement.h"

namespace hls {
namespace m3u8 {

class Tag : public AElement {
public:
    enum class Tag_type {
        m3u8 = 0,
        inf,
        x_byte_range,
        x_target_duration,
        x_media,
        x_stream_inf,
        x_version,
        x_media_sequence,
        x_end_list,
        unknown
    };

    Tag(Tag_type type) : AElement{AElement::Type::tag}, m_type{type} {}

    Tag_type getType() const { return m_type; }

private:
    Tag_type m_type{Tag_type::unknown};
};

} // namespace m3u8
} // namespace hls

#endif