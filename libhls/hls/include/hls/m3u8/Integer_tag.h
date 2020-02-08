#ifndef HLS_M3U8_INTEGERTAG_H_
#define HLS_M3U8_INTEGERTAG_H_

#include "hls/Common.h"
#include "hls/m3u8/Tag.h"

#include "iwu/String_utils.h"

namespace hls {
namespace m3u8 {

class Integer_tag : public Tag {
public:
    Integer_tag(const std::string& value, Tag::Tag_type type) : Tag{type} {
        Expects(iwu::String_utils::is_decimal_number(value),
                Error{"Invalid version string: '"s + value + "'"s});

        m_value = strtoll(value.c_str(), nullptr, 10);
    }

    long value() const { return m_value; }

private:
    long m_value{0};
};

} // namespace m3u8
} // namespace hls

#endif