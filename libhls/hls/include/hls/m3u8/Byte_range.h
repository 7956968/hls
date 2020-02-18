#ifndef HLS_M3U8BYTERANGE_H_
#define HLS_M3U8BYTERANGE_H_

#include "hls/Common.h"

namespace hls {
namespace m3u8 {

/**
 * @brief Byte range as described in RCFC8216,section 4.4.2.2
 */
class Byte_range {
public:
    /**
     * @brief Constructor
     *
     * @param value Raw value in format of <n>[@<o>]
     */
    explicit Byte_range(const std::string& value);

    /**
     * @brief A decimal-integer indicating the length of the sub-range
     * in bytes [RFC8216]
     */
    long length() const;

    /**
     * @brief a decimal-integer indicating the start of the sub-range, as a byte
     * offset from the beginning of the resource [RFC8216]
     */
    nonstd::optional<long> start_byte() const;

private:
    /**
     * @brief See Byte_range::length
     */
    long m_length{-1};

    /**
     * @brief See Byte_range::start_byte
     */
    nonstd::optional<long> m_start_byte;
};

} // namespace m3u8
} // namespace hls

#endif