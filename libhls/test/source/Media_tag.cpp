#include "hls/m3u8/Media_tag.h"
#include "hls/Common.h"

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

constexpr auto k_type_video{"VIDEO"};
constexpr auto k_type_audio{"AUDIO"};
constexpr auto k_type_closed_captions{"CLOSED_CAPTIONS"};
constexpr auto k_type_subtitles{"SUBTITLES"};

const std::map<std::string, hls::m3u8::Media_tag::Type> k_type_map{
  {k_type_audio, hls::m3u8::Media_tag::Type::audio},
  {k_type_video, hls::m3u8::Media_tag::Type::video},
  {k_type_closed_captions, hls::m3u8::Media_tag::Type::closed_captions},
  {k_type_subtitles, hls::m3u8::Media_tag::Type::subtitles},
};

} // namespace

namespace hls {
namespace m3u8 {

Media_tag::Media_tag(const std::string& value) : Tag{Tag::Tag_type::x_media} {
    const Attribute_list al{value};

    const std::string type{
      al.get<Attribute_list::String_enum>(k_attr_type).value};

    auto type_iter{k_type_map.find(type)};
    Expects(type_iter != k_type_map.cend(),
            Error{"Unsupported type: '"s + type + "'"s});
    m_type = type_iter->second;

    m_uri = al.get<std::string>(k_attr_uri);
    m_group_id = al.get<std::string>(k_attr_group_id);
    m_language = al.get<std::string>(k_attr_language);
    m_assoc_language = al.get<std::string>(k_attr_assoc_language);
    m_name = al.get<std::string>(k_attr_name);
    m_default = al.get<bool>(k_attr_default);
    m_auto_select = al.get<bool>(k_attr_auto_select);
    m_forced = al.get<bool>(k_attr_forced);

    m_in_stream_id = al.get<std::string>(k_attr_in_stream_id);
    m_characteristics = al.get<std::string>(k_attr_characteristics);
    m_channels = al.get<std::string>(k_attr_channels);
}

} // namespace m3u8
} // namespace hls