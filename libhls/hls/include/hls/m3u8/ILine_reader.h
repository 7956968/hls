#ifndef HLS_M3U8ILINEREADER_H_
#define HLS_M3U8ILINEREADER_H_

#include <string>

namespace hls {
namespace m3u8 {


/**
 * @brief Reader capable of reading liens one by one from a data source
 */
class ILine_reader {
public:
    /**
     * @brief Read a line
     *
     * @throw End_of_stream if no more lines are available
     */
    virtual std::string read_line() = 0;

    /**
     * @brief Destructor
     */
    virtual ~ILine_reader() = default;
};

} // namespace m3u8

} // namespace hls

#endif