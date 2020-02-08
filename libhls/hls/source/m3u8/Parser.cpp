#include "hls/m3u8/Parser.h"
#include "hls/Common.h"
#include "hls/m3u8/Comment.h"
#include "hls/m3u8/Integer_tag.h"
#include "hls/m3u8/Media_tag.h"
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
    // Register tags
    register_tag_type("M3U", Tag::Tag_type::m3u8)
      .register_tag_type("INF", Tag::Tag_type::inf)
      .register_tag_type("-X-ENDLIST", Tag::Tag_type::x_end_list)
      .register_integer_tag_type("-X-VERSION", Tag::Tag_type::x_version)
      .register_integer_tag_type("-X-MEDIA-SEQUENCE",
                                 Tag::Tag_type::x_media_sequence)
      .register_integer_tag_type("-X-TARGETDURATION",
                                 Tag::Tag_type::x_target_duration)
      .register_specialized_tag_type<Media_tag>("-X-MEDIA",
                                                Tag::Tag_type::x_media)
      .register_specialized_tag_type<Stream_inf_tag>(
        "-X-STREAM-IN", Tag::Tag_type::x_stream_inf);
}

std::unique_ptr<const AElement> Parser::parse_line(const std::string& in_line) {
    std::string line{in_line};

    line = iwu::String_utils::trim(line);
    Expects(!line.empty(), Error{"Empty input"});

    if (iwu::String_utils::starts_with(line, k_tag_prefix)) {
        // Tag
        return parse_tag(line);
    } else if (iwu::String_utils::starts_with(line, k_comment_prefix)) {
        // Comment
        return std::make_unique<Comment>(line.substr(1));
    } else {
        // Uri
        return std::make_unique<Uri>(line);
    }

    return nullptr;
}


Parser& Parser::register_tag_type(const std::string& name, Tag::Tag_type type,
                                  const Tag_parser_fnc& parser) {
    m_registered_tags.insert({name, Tag_entry{name, type, parser}});
    return *this;
}

Parser& Parser::register_integer_tag_type(const std::string& name,
                                          Tag::Tag_type type) {
    return register_tag_type(name, type, [type](const std::string& value) {
        return std::make_unique<Integer_tag>(value, type);
    });
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
