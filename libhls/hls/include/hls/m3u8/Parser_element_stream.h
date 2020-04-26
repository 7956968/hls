#ifndef HLS_M3U8PARSERELEMENTSTREAM_H_
#define HLS_M3U8PARSERELEMENTSTREAM_H_

#include "hls/m3u8/IElement_stream.h"
#include "hls/m3u8/ILine_reader.h"
#include "hls/m3u8/Parser.h"

namespace hls {
namespace m3u8 {

/**
 * @brief Parser element stream which uses a line reader as raw string source,
 * and parses said lines using an IParser
 */
class Parser_element_stream : public IElement_stream {
public:
    /**
     * @brief Constructor
     *
     * @param reader Raw string line reader
     * @param parser String line parser
     */
    Parser_element_stream(std::unique_ptr<ILine_reader> reader,
                          std::unique_ptr<IParser> parser)
        : m_reader{std::move(reader)}, m_parser{std::move(parser)} {}

    /**
     * @brief IElement_stream implementation
     */
public:
    std::unique_ptr<const AElement> get_next() override {
        while (true) {
            std::unique_ptr<const AElement> element{
              m_parser->parse_line(m_reader->read_line())};

            if (!element) {
                // Empty line
                continue;
            }

            return element;
        }
    }

private:
    /**
     * @brief String line reader
     */
    std::unique_ptr<ILine_reader> m_reader;

    /**
     * @brief Line parser
     */
    std::unique_ptr<IParser> m_parser;
};

} // namespace m3u8
} // namespace hls

#endif