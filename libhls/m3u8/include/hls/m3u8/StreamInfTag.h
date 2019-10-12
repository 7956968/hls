#ifndef HLS_M3U8_STREAMINFTAG_H_
#define HLS_M3U8_STREAMINFTAG_H_

#include <hls/m3u8/Tag.h>

namespace hls {

namespace m3u8 {

class StreamInfTag : public Tag {
public:
    StreamInfTag() : Tag(Tag::TagType::X_STREAM_INF), mBandwidth(-1),
                mAverageBandwidth(-1), mCodecs(""), mWidth(-1), mHeight(-1), mFrameRate(-1.0f){
    }

    static Tag* parse(const std::string& value) {
        StreamInfTag* tag = new StreamInfTag;

        AttributeList al;
        al.fromString(value);

        // Mandatory
        if(!al.getInteger("BANDWIDTH", &tag->mBandwidth)) {
            DLOGE("Missing BANDWIDTH attribute");
            return nullptr;
        }

        // Optional
        al.getInteger("AVERAGE-BANDWIDTH", &tag->mAverageBandwidth);

        // Optional
        al.getString("CODECS", &tag->mCodecs);

        std::pair<int32_t, int32_t> resolution;

        if(al.getResolution("RESOLUTION", &resolution)){
            tag->mWidth = resolution.first;
            tag->mHeight = resolution.second;
        }


        al.getFloat("FRAME-RATE", &tag->mFrameRate);

        return tag;
    }

    int64_t mBandwidth;
    int64_t mAverageBandwidth;
    std::string mCodecs;
    int32_t mWidth;
    int32_t mHeight;
    float mFrameRate;
};

} // m3u8

} // hls

#endif
