#include "hls/playlist/media/Parser.h"

namespace hls {
namespace playlist {
namespace media {

std::unique_ptr<Playlist> Parser::parse(m3u8::IElement_stream* stream,
                                        const std::string& uri) {
    Parser parser{stream, uri};
    return parser.parse();
}

Parser::Parser(m3u8::IElement_stream* stream, const std::string& base_uri)
    : APlaylist_parser{stream}, m_base_uri{base_uri} {}

void Parser::parse_next() {
    // Read next element
    std::shared_ptr<const m3u8::AElement> element{stream()->get_next()};

    // First we need an #EXTM3U tag to make sure we're dealing with an HLS
    // playlist
    if (!m_playlist_tags.m3u) {
        Expects(
          element->type() == m3u8::AElement::Type::tag,
          Error{"Expected #EXTM3U tag, got "s + to_string(element->type())});

        auto tag{dynamic_cast<const m3u8::Tag*>(element.get())};
        Expects(tag->type() == m3u8::Tag::Tag_type::m3u8,
                Error{"Expected #EXTM3U tag, got "s + to_string(tag->type())});

        m_playlist_tags.m3u =
          std::dynamic_pointer_cast<const m3u8::Tag>(element);
        Ensures(m_playlist_tags.m3u);

        return;
    }

    Expects(m_playlist_tags.m3u,
            Error{"Elements encountered before #EXTM3U tag"});

    // Did we run into an URI ?
    if (element->type() == m3u8::AElement::Type::uri) {
        // When we reach an URI, this means a segment is finished
        Segment segment(m_media_sequence_number,
                        m_discontinuity_sequence_number,
                        dynamic_cast<const m3u8::Uri*>(element.get())->uri(),
                        m_segment_tags.inf);

        // Set the optional data
        segment.set_gap(m_segment_tags.gap)
          .set_program_date_time(m_segment_tags.program_date_time)
          .set_key(m_segment_shared_tags.key)
          .set_map(m_segment_shared_tags.map);

        if (m_segment_tags.byte_range) {
            segment.set_byte_range(m_segment_tags.byte_range->byte_range());
        }

        if (m_segment_shared_tags.bitrate) {
            segment.set_bitrate(m_segment_shared_tags.bitrate->value());
        }

        m_segments.push_back(segment);
        m_media_sequence_number++;

        // Reset accumulated single segment tags
        m_segment_tags = Segment_tags{};

        // When a first segment is created, we consider the media
        // playlist header to be read
        m_header_read = true;
        return;
    }

    // Handle the incoming tag
    auto tag{dynamic_cast<const m3u8::Tag*>(element.get())};
    Expects(tag);

    switch (tag->type()) {
    // Global playlist tags
    case m3u8::Tag::Tag_type::x_target_duration:
        Expects(!m_playlist_tags.target_duration,
                Error{"Multiple EXT-X-TARGETDURATION tags defined"});

        m_playlist_tags.target_duration =
          std::dynamic_pointer_cast<const m3u8::Integer_tag>(element);
        Ensures(m_playlist_tags.target_duration);
        break;

    case m3u8::Tag::Tag_type::x_media_sequence:
        // This tag must come before first segment [RFC8216b/4.4.3.2]
        Expects(
          !m_header_read,
          Error{"Found X-MEDIA-SEQUENCE tag after segments were defined"});

        m_media_sequence_number =
          dynamic_cast<const m3u8::Integer_tag*>(element.get())->value();

        break;

    case m3u8::Tag::Tag_type::x_discontinuity_sequence:
        // This tag must come before first segment [RFC8216b/4.4.3.3]
        Expects(!m_header_read,
                Error{"Found X-DISCONTINUITY-SEQUENCE tag after "
                      "segments were defined"});

        m_discontinuity_sequence_number =
          dynamic_cast<const m3u8::Integer_tag*>(element.get())->value();

        break;

    case m3u8::Tag::Tag_type::x_end_list:
        // TODO Signals that no more media segments will be added to the
        // playlist. Should we enforce this ? (e.g. if a segment comes
        // after this tag -> throw)
        break;

    case m3u8::Tag::Tag_type::x_playlist_type:
        m_playlist_tags.playlist_type =
          dynamic_cast<const m3u8::Enum_tag<m3u8::Playlist_type>*>(
            element.get())
            ->value();
        break;

    case m3u8::Tag::Tag_type::x_i_frames_only:
        m_playlist_tags.iframes_only = true;
        break;

    case m3u8::Tag::Tag_type::x_independent_segments:
        m_playlist_tags.independent_segments = true;
        break;

    case m3u8::Tag::Tag_type::x_start:
        m_playlist_tags.start =
          std::dynamic_pointer_cast<const m3u8::Start_tag>(element);
        Ensures(m_playlist_tags.start);
        break;

    // Shared segment tags (shared between all incoming segments)
    case m3u8::Tag::Tag_type::x_key:
        m_segment_shared_tags.key =
          std::dynamic_pointer_cast<const m3u8::Key_tag>(element);
        Ensures(m_segment_shared_tags.key);
        break;

    case m3u8::Tag::Tag_type::x_bitrate:
        m_segment_shared_tags.bitrate =
          std::dynamic_pointer_cast<const m3u8::Integer_tag>(element);
        Ensures(m_segment_shared_tags.bitrate);
        break;

    case m3u8::Tag::Tag_type::x_map:
        m_segment_shared_tags.map =
          std::dynamic_pointer_cast<const m3u8::Map_tag>(element);
        Ensures(m_segment_shared_tags.map);
        break;

    case m3u8::Tag::Tag_type::x_discontinuity:
        m_discontinuity_sequence_number++;
        break;

    // Single segment tags
    case m3u8::Tag::Tag_type::inf:
        Expects(!m_segment_tags.inf);

        m_segment_tags.inf =
          std::dynamic_pointer_cast<const m3u8::Inf_tag>(element);
        Ensures(m_segment_tags.inf);
        break;

    case m3u8::Tag::Tag_type::x_byte_range:
        Expects(!m_segment_tags.byte_range);

        m_segment_tags.byte_range =
          std::dynamic_pointer_cast<const m3u8::Byte_range_tag>(element);
        Ensures(m_segment_tags.byte_range);
        break;

    case m3u8::Tag::Tag_type::x_program_date_time:
        Expects(!m_segment_tags.program_date_time);

        m_segment_tags.program_date_time =
          std::dynamic_pointer_cast<const m3u8::Tag>(element);
        Ensures(m_segment_tags.program_date_time);
        break;

    case m3u8::Tag::Tag_type::x_gap:
        m_segment_tags.gap = true;
        break;

    default:
        DILOGW("Unsupported tag type: %s", to_string(tag->type()).c_str());
    }
}

std::unique_ptr<Playlist> Parser::parse() {
    // Parse until stream is depleted
    while (true) {
        try {
            parse_next();
        } catch (const hls::End_of_stream&) { break; }
    }

    // Tag is mandatory [RFC8216b/4.4.3.1]
    Expects(m_playlist_tags.target_duration,
            Error{"Mandatory tag EXT-X-TARGETDURATION not found"});

    m_playlist = std::make_unique<Playlist>(
      m_base_uri,
      std::chrono::seconds(m_playlist_tags.target_duration->value()));

    // Set optional parameters
    m_playlist->set_segments(m_segments)
      .set_iframes_only(m_playlist_tags.iframes_only)
      .set_independent_segments(m_playlist_tags.independent_segments);

    if (m_playlist_tags.start) {
        m_playlist->set_precise_offset(m_playlist_tags.start->precise());

        m_playlist->set_time_offset(
          std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::duration<double>(
              m_playlist_tags.start->time_offset())));
    }

    if (m_playlist_tags.playlist_type) {
        m_playlist->set_type(m_playlist_tags.playlist_type.value());
    }

    return std::move(m_playlist);
}

} // namespace media
} // namespace playlist
} // namespace hls
