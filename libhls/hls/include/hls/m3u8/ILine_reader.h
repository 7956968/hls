#ifndef HLS_M3U8ILINEREADER_H_
#define HLS_M3U8ILINEREADER_H_

#include <string>

namespace hls {
namespace m3u8 {


class ILine_reader {
public:
    virtual std::string read_line() = 0;

    virtual ~ILine_reader() = default;
};

} // namespace m3u8

} // namespace hls

#endif