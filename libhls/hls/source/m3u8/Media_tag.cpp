#include "hls/m3u8/Media_tag.h"
#include "hls/Common.h"
#include "hls/m3u8/Common.h"

namespace {

constexpr auto k_attr_type{"TYPE"};
constexpr auto k_attr_uri{"URI"};
constexpr auto k_attr_group_id{"GROUP-ID"};
constexpr auto k_attr_language{"LANGUAGE"};
constexpr auto k_attr_assoc_language{"ASSOC-LANGUAGE"};
constexpr auto k_attr_name{"NAME"};
constexpr auto k_attr_default{"DEFAULT"};
constexpr auto k_attr_auto_select{"AUTOSELECT"};
constexpr auto k_attr_forced{"FORCED"};
constexpr auto k_attr_in_stream_id{"INSTREAM-ID"};
constexpr auto k_attr_characteristics{"CHARACTERISTICS"};
constexpr auto k_attr_channels{"CHANNELS"};

} // namespace

namespace hls {
namespace m3u8 {

std::function<Media_tag::Media_type(const std::string&)>
  Media_tag::s_media_type_parser{[](const std::string& name) {
      return hls::m3u8::parse_enum_string(
        std::map<std::string, Media_tag::Media_type>{
          {"AUDIO", Media_type::audio},
          {"VIDEO", Media_type::video},
          {"CLOSED-CAPTIONS", Media_type::closed_captions},
          {"SUBTITLES", Media_type::subtitles}},
        name);
  }};

Media_tag::Media_tag(const std::string& value) : Tag{Tag::Tag_type::x_media} {
    const Attribute_list al{value};

    // Mandatory attributes
    m_media_type = al.get_parsed_enum_string(s_media_type_parser, k_attr_type);
    m_group_id = al.get<std::string>(k_attr_group_id);
    m_name = al.get<std::string>(k_attr_name);

    // Optional attributes
    if (al.contains(k_attr_uri)) {
        m_uri = al.get<std::string>(k_attr_uri);
    }

    if (al.contains(k_attr_language)) {
        m_language = al.get<std::string>(k_attr_language);
    }

    if (al.contains(k_attr_assoc_language)) {
        m_assoc_language = al.get<std::string>(k_attr_assoc_language);
    }

    if (al.contains(k_attr_default)) {
        m_default = al.get<bool>(k_attr_default);
    }

    if (al.contains(k_attr_auto_select)) {
        m_auto_select = al.get<bool>(k_attr_auto_select);
    }

    if (al.contains(k_attr_forced)) {
        m_forced = al.get<bool>(k_attr_forced);
    }

    if (al.contains(k_attr_in_stream_id)) {
        m_in_stream_id = al.get<std::string>(k_attr_in_stream_id);
    }

    if (al.contains(k_attr_characteristics)) {
        m_characteristics =
          al.get_delim_separated_string(k_attr_characteristics, ","s);
    }

    if (al.contains(k_attr_channels)) {
        m_channels = al.get_delim_separated_string(k_attr_channels, "/"s);
    }
}

Media_tag::Media_type Media_tag::media_type() const { return m_media_type; }

nonstd::optional<std::string> Media_tag::uri() const { return m_uri; }

const std::string& Media_tag::group_id() const { return m_group_id; }

nonstd::optional<std::string> Media_tag::language() const { return m_language; }

nonstd::optional<std::string> Media_tag::assoc_language() const {
    return m_assoc_language;
}

const std::string& Media_tag::name() const { return m_name; }

bool Media_tag::is_default() const { return m_default; }

bool Media_tag::auto_select() const { return m_auto_select; }

bool Media_tag::forced() const { return m_forced; }

nonstd::optional<std::string> Media_tag::in_stream_id() const {
    return m_in_stream_id;
}

nonstd::optional<std::vector<std::string>> Media_tag::characteristics() const {
    return m_characteristics;
}

nonstd::optional<std::vector<std::string>> Media_tag::channels() const {
    return m_channels;
}

} // namespace m3u8
} // namespace hls