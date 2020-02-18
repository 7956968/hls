#include "hls/m3u8/Map_tag.h"

#include "hls/m3u8/Attribute_list.h"

namespace {
constexpr auto k_attr_uri{"URI"};
constexpr auto k_attr_byte_range{"BYTERANGE"};
} // namespace

namespace hls {
namespace m3u8 {

Map_tag::Map_tag(const std::string& value) : Tag{Tag_type::x_map} {
    const Attribute_list al{value};

    m_uri = al.get<std::string>(k_attr_uri);

    if (al.contains(k_attr_byte_range)) {
        m_byte_range = Byte_range{al.get<std::string>(k_attr_byte_range)};
    }
}

const std::string& Map_tag::uri() const { return m_uri; }

nonstd::optional<Byte_range> Map_tag::byte_range() const {
    return m_byte_range;
}

} // namespace m3u8
} // namespace hls
