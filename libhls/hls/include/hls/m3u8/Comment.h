#ifndef HLS_M3U8_COMMENT_H_
#define HLS_M3U8_COMMENT_H_

#include "hls/m3u8/AElement.h"

#include <string>

namespace hls {
namespace m3u8 {

class Comment : public AElement {
public:
    Comment(const std::string& body)
        : AElement{AElement::Type::comment}, m_body{body} {}

    const std::string& body() const { return m_body; }

private:
    std::string m_body;
};

} // namespace m3u8
} // namespace hls

#endif