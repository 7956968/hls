#ifndef HLS_M3U8_INTEGERTAG_H_
#define HLS_M3U8_INTEGERTAG_H_

#include <hls/m3u8/Tag.h>

#include <iwu/StringUtils.h>

namespace hls {

namespace m3u8 {

class IntegerTag : public Tag {
public:
    IntegerTag(Tag::TagType type): Tag(type), mValue(-1) {
    }

    bool parse(const std::string& value){
        if(!iwu::StringUtils::isDecimalNumber(value)) {
            return false;
        }

        mValue = atoi(value.c_str());

        return true;
    }

    int32_t getValue() const {
        return mValue;
    }

private:
    int32_t mValue;
};

} // m3u8

} // hls

#endif