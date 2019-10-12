#ifndef HLS_M3U8_TAG_H_
#define HLS_M3U8_TAG_H_

#include <hls/m3u8/AElement.h>

namespace hls {

namespace m3u8 {

class Tag : public AElement {
public:
   enum class TagType {
        M3U = 0,
        INF,
        X_BYTE_RANGE,
        X_TARGET_DURATION,
        X_MEDIA,
        X_STREAM_INF,
        X_VERSION,
        X_MEDIA_SEQUENCE,
        X_END_LIST,
        UNKOWN
    };

    Tag(TagType type) : AElement(AElement::Type::TAG),
        mType(type) {
    }

    TagType getType() const {
        return mType;
    }
private:
    TagType mType;
};

} // m3u8

} // hls

#endif