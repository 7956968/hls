#ifndef HLS_M3U8STRINGLINEREADER_H_
#define HLS_M3U8STRINGLINEREADER_H_

#include "hls/Common.h"
#include "hls/m3u8/ILine_reader.h"

namespace hls {
namespace m3u8 {

/**
 * @brief Line reader which takes a plain string as a source

 */
class String_line_reader : public ILine_reader {
public:
    /**
     * @brief Constructor
     *
     * @param source String data source
     */
    explicit String_line_reader(const std::string& source) : m_source{source} {}

    /**
     * @brief ILine_reader implementation
     */
public:
    std::string read_line() override {
        if (m_source.empty()) {
            throw End_of_stream{};
        }

        // TODO Add support for \r\n style new lines
        size_t new_line_pos{m_source.find('\n')};
        if (new_line_pos == std::string::npos) {
            new_line_pos = m_source.size();
        }

        const std::string res{m_source.substr(0, new_line_pos)};

        m_source = m_source.substr(std::min(m_source.size(), new_line_pos + 1));

        return res;
    }

private:
    /**
     * @brief Data source

     */
    std::string m_source;
};


} // namespace m3u8
} // namespace hls

#endif