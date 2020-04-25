#include "hls/m3u8/Parser.h"
#include "hls/Common.h"
#include "hls/m3u8/Byte_range_tag.h"
#include "hls/m3u8/Comment.h"
#include "hls/m3u8/Inf_tag.h"
#include "hls/m3u8/Integer_tag.h"
#include "hls/m3u8/Key_tag.h"
#include "hls/m3u8/Map_tag.h"
#include "hls/m3u8/Media_tag.h"
#include "hls/m3u8/Start_tag.h"
#include "hls/m3u8/Stream_inf_tag.h"
#include "hls/m3u8/Uri.h"

#include "iwu/String_utils.h"

namespace {
const auto k_tag_prefix{"#EXT"s};
constexpr auto k_comment_prefix{"#"};
} // namespace

namespace hls {
namespace m3u8 {


Parser::Parser() {
    // Basic tags
    register_tag_type("M3U", Tag::Tag_type::m3u8)
      .register_integer_tag_type("-X-VERSION", Tag::Tag_type::x_version);

    // Media segment tags
    // Each Media Segment is specified by a series of Media Segment tags
    // followed by a URI.  Some Media Segment tags apply to just the next
    // segment; others apply to all subsequent segments until another instance
    // of the same tag [RFC8216]
    register_specialized_tag_type<Inf_tag>("INF", Tag::Tag_type::inf)
      .register_specialized_tag_type<Byte_range_tag>(
        "-X-BYTERANGE", Tag::Tag_type::x_byte_range)
      .register_tag_type("-X-DISCONTINUITY", Tag::Tag_type::x_discontinuity)
      .register_specialized_tag_type<Key_tag>("-X-KEY", Tag::Tag_type::x_key)
      .register_specialized_tag_type<Map_tag>("-X-MAP", Tag::Tag_type::x_map)
      // TODO Parse correctly
      .register_tag_type("-X-PROGRAM-DATE-TIME",
                         Tag::Tag_type::x_program_date_time)
      .register_tag_type("-X-GAP", Tag::Tag_type::x_gap)
      .register_integer_tag_type("-X-BITRATE", Tag::Tag_type::x_bitrate);

    // Media metadata tags
    register_tag_type("-X-DATERANGE", Tag::Tag_type::x_date_range);

    // Media playlist tags
    register_integer_tag_type("-X-TARGETDURATION",
                              Tag::Tag_type::x_target_duration)
      .register_integer_tag_type("-X-MEDIA-SEQUENCE",
                                 Tag::Tag_type::x_media_sequence)
      .register_integer_tag_type("-X-DISCONTINUITY-SEQUENCE",
                                 Tag::Tag_type::x_discontinuity_sequence)
      .register_tag_type("-X-ENDLIST", Tag::Tag_type::x_end_list)
      .register_tag_type("-X-I-FRAMES-ONLY", Tag::Tag_type::x_i_frames_only)
      .register_enum_tag("-X-PLAYLIST-TYPE", Tag::Tag_type::x_playlist_type,
                         get_playlist_type_parser());

    // Master playlist tags
    register_specialized_tag_type<Media_tag>("-X-MEDIA", Tag::Tag_type::x_media)
      .register_specialized_tag_type<Stream_inf_tag>(
        "-X-STREAM-INF", Tag::Tag_type::x_stream_inf)
      .register_tag_type("-X-I-FRAME-STREAM-INF",
                         Tag::Tag_type::x_i_frame_stream_inf)
      .register_tag_type("-X-SESSION-DATA", Tag::Tag_type::x_session_data)
      .register_specialized_tag_type<Key_tag>("-X-SESSION-KEY",
                                              Tag::Tag_type::x_session_key);

    // Media or master playlist tags
    register_tag_type("-X-INDEPENDENT-SEGMENTS",
                      Tag::Tag_type::x_independent_segments)
      .register_specialized_tag_type<Start_tag>("-X-START",
                                                Tag::Tag_type::x_start)
      // TODO Parse properly
      .register_tag_type("-X-DEFINE", Tag::Tag_type::x_define);
}

std::unique_ptr<const AElement> Parser::parse_line(const std::string& in_line) {
    // Trim left whitespace
    std::string line{iwu::String_utils::left_trim(in_line)};

    // Trim right new lines
    auto new_line_chars{[](char c) {
        return c == '\n' || c == '\r';
    }};
    line = iwu::String_utils::right_trim(line, new_line_chars);

    if (line.empty()) {
        return nullptr;
    } else if (iwu::String_utils::starts_with(line, k_tag_prefix)) {
        // Tag
        return parse_tag(line);
    } else if (iwu::String_utils::starts_with(line, k_comment_prefix)) {
        // Comment
        return std::make_unique<Comment>(line.substr(1));
    } else {
        // Uri
        return std::make_unique<Uri>(line);
    }
}

Parser& Parser::register_tag_type(const std::string& name, Tag::Tag_type type,
                                  const Tag_parser_fnc& parser) {
    return register_tag_type(name, Tag_entry{name, type, parser});
}

Parser& Parser::register_integer_tag_type(const std::string& name,
                                          Tag::Tag_type type) {
    return register_tag_type(name, type, [type](const std::string& value) {
        return std::make_unique<Integer_tag>(value, type);
    });
}

Parser& Parser ::register_tag_type(const std::string& name, Tag_entry&& entry) {
    Expects(!name.empty());
    Expects(m_registered_tags.find(name) == m_registered_tags.end());

    m_registered_tags.insert({name, std::forward<Tag_entry>(entry)});

    return *this;
}

std::unique_ptr<Tag> Parser::parse_tag(const std::string& in_line) {
    std::string line{in_line};

    // Find tag name end
    const size_t tag_value_start_pos{line.find(':')};

    const bool has_value{tag_value_start_pos != std::string::npos};

    const size_t tag_name_end_pos{has_value ? tag_value_start_pos
                                            : line.size()};

    // Extract name
    const std::string tag_name{
      line.substr(k_tag_prefix.size(), tag_name_end_pos - k_tag_prefix.size())};


    line = has_value ? line.substr(tag_name_end_pos + 1) : "";

    // Find parser by name
    auto tag_iter{m_registered_tags.find(tag_name)};
    Expects(tag_iter != m_registered_tags.end(),
            Error("Unsupported tag: '"s + tag_name + "'"s));

    const Tag_entry& entry{tag_iter->second};
    // Parse if needed
    if (entry.parser) {
        return entry.parser(line);
    }

    return std::make_unique<Tag>(entry.type);
}

} // namespace m3u8
} // namespace hls
