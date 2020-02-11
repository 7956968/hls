#ifndef HLS_M3U8_AELEMENT_H_
#define HLS_M3U8_AELEMENT_H_

#include "hls/Common.h"

namespace hls {
namespace m3u8 {

class AElement {
public:
    enum class Type { comment, tag, uri, unknown };

public:
    explicit AElement(Type type) : m_type{type} {}

    virtual ~AElement() = default;

    Type type() const { return m_type; }

private:
    Type m_type;
};

} // namespace m3u8

inline std::string to_string(m3u8::AElement::Type type) {
    switch (type) {
    case m3u8::AElement::Type::comment:
        return "m3u8::AElement::Type::comment"s;

    case m3u8::AElement::Type::tag:
        return "m3u8::AElement::Type::tag"s;

    case m3u8::AElement::Type::uri:
        return "m3u8::AElement::Type::uri"s;

    case m3u8::AElement::Type::unknown:
        return "m3u8::AElement::Type::unknown"s;
    }
}
} // namespace hls

#endif