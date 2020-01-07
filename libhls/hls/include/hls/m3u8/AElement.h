#ifndef HLS_M3U8_AELEMENT_H_
#define HLS_M3U8_AELEMENT_H_

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
} // namespace hls

#endif