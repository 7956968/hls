#ifndef HLS_M3U8_Media_tag_H_
#define HLS_M3U8_Media_tag_H_

#include "hls/m3u8/Attribute_list.h"
#include "hls/m3u8/Tag.h"

namespace hls {
namespace m3u8 {

class Media_tag : public Tag {
public:
    enum class Type { audio, video, subtitles, closed_captions, unknown };

    explicit Media_tag(const std::string& value);

private:
    Type m_type{Type::unknown};
    std::string m_uri;
    std::string m_group_id;
    std::string m_language;
    std::string m_assoc_language;
    std::string m_name;
    bool m_default{false};
    bool m_auto_select{false};
    bool m_forced{false};
    std::string m_in_stream_id;
    std::string m_characteristics;
    std::string m_channels;
};

} // namespace m3u8
} // namespace hls

#endif
