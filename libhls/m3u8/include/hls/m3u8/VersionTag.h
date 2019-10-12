#ifndef HLS_M3U8_VERSIONTAG_H_
#define HLS_M3U8_VERSIONTAG_H_

#include <hls/m3u8/Tag.h>

namespace hls {

namespace m3u8 {

class VersionTag : public Tag {
public:
    static Tag* parse(const std::string& value) {
        return new VersionTag(atoi(value.c_str()));
    }

private:
    VersionTag(int32_t version) : Tag(TagType::X_VERSION), mVersion(version){
    }

private:
    int32_t mVersion;
};

} // m3u8

} // hls

#endif
