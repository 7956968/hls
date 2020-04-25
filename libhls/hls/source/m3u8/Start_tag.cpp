#include "hls/m3u8/Start_tag.h"

namespace {

constexpr auto k_attr_precise{"PRECISE"};
constexpr auto k_attr_time_offset{"TIME-OFFSET"};

} // namespace

namespace hls {
namespace m3u8 {


Start_tag::Start_tag(const std::string& value) : Tag{Tag::Tag_type::x_start} {
    const Attribute_list al{value};

    m_time_offset = al.get<float>(k_attr_time_offset);

    if (al.contains(k_attr_precise)) {
        m_precise = al.get<bool>(k_attr_precise);
    }
}

float Start_tag::time_offset() const { return m_time_offset; }

bool Start_tag::precise() const { return m_precise; }

} // namespace m3u8
} // namespace hls