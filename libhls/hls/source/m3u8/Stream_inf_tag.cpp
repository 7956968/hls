#include "hls/m3u8/Stream_inf_tag.h"

namespace {
constexpr auto k_attr_bandwidth{"BANDWIDTH"};
constexpr auto k_attr_average_bandwidth{"AVERAGE-BANDWIDTH"};
constexpr auto k_attr_codecs{"CODECS"};
constexpr auto k_attr_resolution{"RESOLUTION"};
constexpr auto k_attr_frame_rate{"FRAME-RATE"};

} // namespace

namespace hls {
namespace m3u8 {

Stream_inf_tag::Stream_inf_tag(const std::string& value)
    : Tag{Tag::Tag_type::x_stream_inf} {
    const Attribute_list al{value};

    m_bandwidth = al.get<long long>(k_attr_bandwidth);

    if (al.contains(k_attr_average_bandwidth)) {
        m_average_bandwidth = al.get<long long>(k_attr_average_bandwidth);
    }

    if (al.contains(k_attr_codecs)) {
        m_codecs = al.get<std::string>(k_attr_codecs);
    }

    if (al.contains(k_attr_resolution)) {
        m_resolution = al.get<Attribute_list::Resolution>(k_attr_resolution);
    }

    m_frame_rate = al.get<float>(k_attr_frame_rate);
}

long Stream_inf_tag::bandwidth() const { return m_bandwidth; }

long Stream_inf_tag::average_bandwidth() const { return m_average_bandwidth; }

const std::string& Stream_inf_tag::codecs() const { return m_codecs; }

Attribute_list::Resolution Stream_inf_tag::resolution() const {
    return m_resolution;
}

float Stream_inf_tag::frame_rate() const { return m_frame_rate; }

} // namespace m3u8
} // namespace hls
