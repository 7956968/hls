#ifndef HLS_M3U8ENUMTAG_H_
#define HLS_M3U8ENUMTAG_H_


#include "hls/Common.h"
#include "hls/m3u8/Common.h"
#include "hls/m3u8/Tag.h"

#include "iwu/String_utils.h"

namespace hls {
namespace m3u8 {

/**
 * @brief Generic enumeration tag
 *
 * @tparam T Enum class
 */
template<typename T>
class Enum_tag : public Tag {
public:
    Enum_tag(const std::string& value, Tag::Tag_type type,
             std::function<T(const std::string&)> parser)
        : Tag{type}, m_value{parser(value)} {}

    /**
     * @brief Enum value
     */
    T value() const { return m_value; }

private:
    /**
     * @brief See Enum_tag::value
     */
    T m_value;
};


} // namespace m3u8
} // namespace hls

#endif