#include "hls/m3u8/Stream_inf_tag.h"
#include "hls/m3u8/Common.h"

namespace {
constexpr auto k_attr_bandwidth{"BANDWIDTH"};
constexpr auto k_attr_average_bandwidth{"AVERAGE-BANDWIDTH"};
constexpr auto k_attr_codecs{"CODECS"};
constexpr auto k_attr_resolution{"RESOLUTION"};
constexpr auto k_attr_frame_rate{"FRAME-RATE"};
constexpr auto k_attr_hdcp_level{"HDCP-LEVEL"};
constexpr auto k_attr_allowed_cpc{"ALLOWED-CPC"};
constexpr auto k_attr_video_range{"VIDEO-RANGE"};
constexpr auto k_attr_audio{"AUDIO"};
constexpr auto k_attr_video{"VIDEO"};
constexpr auto k_attr_subtitles{"SUBTITLES"};
constexpr auto k_attr_closed_captions{"CLOSED-CAPTIONS"};
constexpr auto k_attr_closed_captions_none{"NONE"};
} // namespace

namespace hls {
namespace m3u8 {


std::function<Stream_inf_tag::Hdcp_level(const std::string&)>
  Stream_inf_tag::s_hdcp_level_parser{[](const std::string& name) {
      return hls::m3u8::parse_enum_string(
        std::map<std::string, Hdcp_level>{{"TYPE-0", Hdcp_level::type_0},
                                          {"TYPE-1", Hdcp_level::type_1},
                                          {"TYPE-NONE", Hdcp_level::none}},
        name);
  }};

std::function<Stream_inf_tag::Video_range(const std::string&)>
  Stream_inf_tag::s_video_range_parser{[](const std::string& name) {
      return hls::m3u8::parse_enum_string(
        std::map<std::string, Video_range>{{"SDR", Video_range::sdr},
                                           {"PQ", Video_range::pq}},
        name);
  }};

Stream_inf_tag::Stream_inf_tag(const std::string& value)
    : Tag{Tag::Tag_type::x_stream_inf} {
    const Attribute_list al{value};

    // Every EXT-X-STREAM-INF tag MUST include the BANDWIDTH attribute (source
    // rfc8216)
    m_bandwidth = al.get<long long>(k_attr_bandwidth);

    if (al.contains(k_attr_average_bandwidth)) {
        m_average_bandwidth = al.get<long long>(k_attr_average_bandwidth);
    }

    if (al.contains(k_attr_codecs)) {
        m_codecs = al.get_delim_separated_string(k_attr_codecs, ",");
    }

    if (al.contains(k_attr_resolution)) {
        m_resolution = al.get<Attribute_list::Resolution>(k_attr_resolution);
    }

    if (al.contains(k_attr_frame_rate)) {
        m_frame_rate = al.get<float>(k_attr_frame_rate);
    }

    if (al.contains(k_attr_hdcp_level)) {
        m_hdcp_level =
          al.get_parsed_enum_string(s_hdcp_level_parser, k_attr_hdcp_level);
    }

    if (al.contains(k_attr_allowed_cpc)) {
        m_allowed_cpc = al.get_delim_separated_string(k_attr_allowed_cpc, ",");
    }

    if (al.contains(k_attr_video_range)) {
        m_video_range =
          al.get_parsed_enum_string(s_video_range_parser, k_attr_video_range);
    }

    if (al.contains(k_attr_audio)) {
        m_audio = al.get<std::string>(k_attr_audio);
    }

    if (al.contains(k_attr_video)) {
        m_video = al.get<std::string>(k_attr_video);
    }

    if (al.contains(k_attr_subtitles)) {
        m_subtitles = al.get<std::string>(k_attr_subtitles);
    }

    if (al.contains(k_attr_closed_captions)) {
        try {
            m_closed_captions = {true,
                                 al.get<std::string>(k_attr_closed_captions)};
        } catch (const mpark::bad_variant_access&) {
            const Attribute_list::String_enum str_enum{
              al.get<Attribute_list::String_enum>(k_attr_closed_captions)};

            Expects(str_enum.value == k_attr_closed_captions_none);

            m_closed_captions = {false, ""s};
        }
    }
}


long Stream_inf_tag::bandwidth() const { return m_bandwidth; }

nonstd::optional<long> Stream_inf_tag::average_bandwidth() const {
    return m_average_bandwidth;
}

nonstd::optional<std::vector<std::string>> Stream_inf_tag::codecs() const {
    return m_codecs;
}

nonstd::optional<Attribute_list::Resolution>
Stream_inf_tag::resolution() const {
    return m_resolution;
}

nonstd::optional<float> Stream_inf_tag::frame_rate() const {
    return m_frame_rate;
}


nonstd::optional<Stream_inf_tag::Hdcp_level>
Stream_inf_tag::hdcp_level() const {
    return m_hdcp_level;
}

nonstd::optional<std::vector<std::string>> Stream_inf_tag::allowed_cpc() const {
    return m_allowed_cpc;
}

nonstd::optional<Stream_inf_tag::Video_range>
Stream_inf_tag::video_range() const {
    return m_video_range;
}

nonstd::optional<std::string> Stream_inf_tag::audio() const { return m_audio; }

nonstd::optional<std::string> Stream_inf_tag::video() const { return m_video; }

nonstd::optional<std::string> Stream_inf_tag::subtitles() const {
    return m_subtitles;
}

nonstd::optional<std::pair<bool, std::string>>
Stream_inf_tag::closed_captions() const {
    return m_closed_captions;
}

} // namespace m3u8
} // namespace hls
