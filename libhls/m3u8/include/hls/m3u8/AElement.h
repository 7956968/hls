#ifndef HLS_M3U8_AELEMENT_H_
#define HLS_M3U8_AELEMENT_H_

namespace hls {


namespace m3u8 {

class AElement {
public:
    enum class Type {
        COMMENT,
        TAG,
        URI,
        UNKOWN
    };

public:
    AElement(Type type) : mType(type) {
    }

    virtual ~AElement() {
    }

    const Type& getType() const {
        return mType;
    }

private:
    Type mType;
};

} // m3u8

} // hls

#endif