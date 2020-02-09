#ifndef HLS_M3U8_TAG_H_
#define HLS_M3U8_TAG_H_

#include "hls/Common.h"
#include "hls/m3u8/AElement.h"

namespace hls {
namespace m3u8 {

class Tag : public AElement {
public:
    enum class Tag_type {
        // Basic tags
        m3u8 = 0,
        x_version,

        // Media segment tags
        inf,
        x_byte_range,
        x_discontinuity,
        x_key,
        x_map,
        x_program_date_time,
        x_gap,
        x_bitrate,

        // Media metadata tags
        x_date_range,

        // Media playlist tags
        x_target_duration,
        x_media_sequence,
        x_discontinuity_sequence,
        x_end_list,
        x_playlist_type,
        x_i_frames_only,

        // Master playlist tags
        x_media,
        x_stream_inf,
        x_i_frame_stream_inf,
        x_session_data,
        x_session_key,

        // Media or master playlist tags
        x_independent_segments,
        x_start,
        x_define,

        unknown
    };

    Tag(Tag_type type) : AElement{AElement::Type::tag}, m_type{type} {}

    Tag_type type() const { return m_type; }

private:
    Tag_type m_type{Tag_type::unknown};
};

} // namespace m3u8

inline std::string to_string(m3u8::Tag::Tag_type type) {
    switch (type) {
    case m3u8::Tag::Tag_type::m3u8:
        return "m3u8::Tag::Tag_type::m3u8"s;
    case m3u8::Tag::Tag_type::x_version:
        return "m3u8::Tag::Tag_type::"s;
    case m3u8::Tag::Tag_type::inf:
        return "m3u8::Tag::Tag_type::Tag_type"s;
    case m3u8::Tag::Tag_type::x_byte_range:
        return "m3u8::Tag::Tag_type::x_byte_range"s;
    case m3u8::Tag::Tag_type::x_discontinuity:
        return "m3u8::Tag::Tag_type::x_discontinuity"s;
    case m3u8::Tag::Tag_type::x_key:
        return "m3u8::Tag::Tag_type::x_key"s;
    case m3u8::Tag::Tag_type::x_map:
        return "m3u8::Tag::Tag_type::x_map"s;
    case m3u8::Tag::Tag_type::x_program_date_time:
        return "m3u8::Tag::Tag_type::x_program_date_time"s;
    case m3u8::Tag::Tag_type::x_gap:
        return "m3u8::Tag::Tag_type::x_gap"s;
    case m3u8::Tag::Tag_type::x_bitrate:
        return "m3u8::Tag::Tag_type::x_bitrate"s;
    case m3u8::Tag::Tag_type::x_target_duration:
        return "m3u8::Tag::Tag_type::x_target_duration"s;
    case m3u8::Tag::Tag_type::x_date_range:
        return "m3u8::Tag::Tag_type::x_date_range"s;
    case m3u8::Tag::Tag_type::x_media_sequence:
        return "m3u8::Tag::Tag_type::x_media_sequence"s;
    case m3u8::Tag::Tag_type::x_discontinuity_sequence:
        return "m3u8::Tag::Tag_type::x_discontinuity_sequence"s;
    case m3u8::Tag::Tag_type::x_playlist_type:
        return "m3u8::Tag::Tag_type::x_playlist_type"s;
    case m3u8::Tag::Tag_type::x_end_list:
        return "m3u8::Tag::Tag_type::x_end_list"s;
    case m3u8::Tag::Tag_type::x_i_frames_only:
        return "m3u8::Tag::Tag_type::x_i_frames_only"s;
    case m3u8::Tag::Tag_type::x_media:
        return "m3u8::Tag::Tag_type::x_media"s;
    case m3u8::Tag::Tag_type::x_stream_inf:
        return "m3u8::Tag::Tag_type::x_stream_inf"s;
    case m3u8::Tag::Tag_type::x_i_frame_stream_inf:
        return "m3u8::Tag::Tag_type::x_i_frame_stream_inf"s;
    case m3u8::Tag::Tag_type::x_session_data:
        return "m3u8::Tag::Tag_type::x_session_data"s;
    case m3u8::Tag::Tag_type::x_session_key:
        return "m3u8::Tag::Tag_type::x_session_key"s;
    case m3u8::Tag::Tag_type::x_independent_segments:
        return "m3u8::Tag::Tag_type::x_independent_segments"s;
    case m3u8::Tag::Tag_type::x_start:
        return "m3u8::Tag::Tag_type::x_start"s;
    case m3u8::Tag::Tag_type::x_define:
        return "m3u8::Tag::Tag_type::x_define"s;
    case m3u8::Tag::Tag_type::unknown:
        return "m3u8::Tag::Tag_type::unknown"s;
    }
}

} // namespace hls

#endif