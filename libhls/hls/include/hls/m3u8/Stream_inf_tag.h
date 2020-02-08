#ifndef HLS_M3U8_STREAMINFTAG_H_
#define HLS_M3U8_STREAMINFTAG_H_

#include "hls/m3u8/Attribute_list.h"
#include "hls/m3u8/Tag.h"

namespace hls {
namespace m3u8 {

class Stream_inf_tag : public Tag {
public:
    Stream_inf_tag(const std::string& value);

    long bandwidth() const;

    long average_bandwidth() const;

    const std::string& codecs() const;

    Attribute_list::Resolution resolution() const;

    float frame_rate() const;

private:
    long m_bandwidth{-1};
    long m_average_bandwidth{-1};
    std::string m_codecs;
    Attribute_list::Resolution m_resolution;
    float m_frame_rate{-1};
}; // namespace m3u8

} // namespace m3u8
} // namespace hls

#endif
