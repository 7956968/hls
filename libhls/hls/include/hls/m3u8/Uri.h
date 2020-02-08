#ifndef HLS_M3U8_URI_H_
#define HLS_M3U8_URI_H_

#include "hls/m3u8/AElement.h"

#include <string>

namespace hls {
namespace m3u8 {

class Uri : public AElement {
public:
    Uri(const std::string& uri) : AElement{AElement::Type::uri}, m_uri(uri) {}

    const std::string& uri() const { return m_uri; }

private:
    std::string m_uri;
};

} // namespace m3u8
} // namespace hls

#endif