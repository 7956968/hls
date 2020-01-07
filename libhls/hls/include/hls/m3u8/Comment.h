#ifndef HLS_M3U8_COMMENT_H_
#define HLS_M3U8_COMMENT_H_

#include "hls/m3u8/AElement.h"

#include <string>

namespace hls {

namespace m3u8 {


class Comment : public AElement {
public:
    Comment(const std::string& body) : AElement(AElement::Type::COMMENT), mBody(body) {

    }

    const std::string& getBody() const {
        return mBody;
    }

private:
    std::string mBody;
};

} // m3u8

} // hls

#endif