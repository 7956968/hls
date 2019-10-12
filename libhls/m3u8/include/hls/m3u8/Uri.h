#ifndef HLS_M3U8_URI_H_
#define HLS_M3U8_URI_H_

namespace hls {

namespace m3u8 {

class Uri : public AElement {
public:
    Uri(const std::string& uri) : AElement(AElement::Type::URI), mUri(uri) {
    }

    const std::string& getUri() const {
        return mUri;
    }
private:
    std::string mUri;
};

} // m3u8

} // hls

#endif