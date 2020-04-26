#ifndef HLS_M3U8_INTEGERTAG_H_
#define HLS_M3U8_INTEGERTAG_H_

#include "hls/Common.h"
#include "hls/m3u8/Tag.h"

#include "iwu/String_utils.h"

namespace hls {
namespace m3u8 {

/**
 * @brief Generic integer tag (i.e. any tag which only has an integer for a
 * value)
 */
class Integer_tag : public Tag {
public:
    Integer_tag(const std::string& value, Tag::Tag_type type) : Tag{type} {
        Expects(iwu::String_utils::is_decimal_number(value),
                Error{"Invalid integer string: '"s + value + "'"s});

        m_value = strtoll(value.c_str(), nullptr, 10);
    }

    /**
     * @brief Tag value
     */
    long value() const { return m_value; }

private:
    /**
     * @brief See Integer_tag::value
     */
    long m_value{0};
};

} // namespace m3u8
} // namespace hls

#endif