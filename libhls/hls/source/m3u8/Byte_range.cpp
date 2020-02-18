#include "hls/m3u8/Byte_range.h"

#include "iwu/String_utils.h"

namespace hls {
namespace m3u8 {

Byte_range::Byte_range(const std::string& value) {
    std::vector<std::string> tokens;

    iwu::String_utils::split(value, "@", tokens);

    Expects(tokens.size() == 1 || tokens.size() == 2);

    // Parse length
    const std::string& length_str{tokens[0]};
    Expects(iwu::String_utils::is_decimal_number(length_str));
    m_length = strtol(length_str.c_str(), nullptr, 10);

    // Parse start byte
    if (tokens.size() == 2) {
        const std::string& start_byte_str{tokens[1]};
        Expects(iwu::String_utils::is_decimal_number(start_byte_str));
        m_start_byte = strtol(start_byte_str.c_str(), nullptr, 10);
    }
}

long Byte_range::length() const { return m_length; }

nonstd::optional<long> Byte_range::start_byte() const { return m_start_byte; }

} // namespace m3u8
} // namespace hls
