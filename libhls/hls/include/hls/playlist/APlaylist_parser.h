#ifndef HLS_PLAYLISTAPLALISTPARSER_H_
#define HLS_PLAYLISTAPLALISTPARSER_H_

#include "hls/m3u8/IElement_stream.h"
#include "hls/m3u8/Tag.h"

namespace hls {
namespace playlist {

/**
 * @brief Base class for all playlist parsers
 */
class APlaylist_parser {
public:
    /**
     * @brief Constructor
     *
     * @param stream M3U8 element stream
     */
    explicit APlaylist_parser(m3u8::IElement_stream* stream)
        : m_stream{stream} {}

    /**
     * @brief Element stream
     */
    m3u8::IElement_stream* stream() { return m_stream; }

    /**
     * @brief
     *
     * @tparam T
     * @param type
     * @return std::shared_ptr<const T>
     */
    template<typename T>
    std::shared_ptr<const T>
    read_tag(m3u8::Tag::Tag_type type = m3u8::Tag::Tag_type::unknown) {
        // Read the tag
        std::shared_ptr<const m3u8::AElement> element{m_stream->get_next()};

        // Check that it's indeed a tag
        Expects(element->type() == m3u8::AElement::Type::tag);

        // If type is specified, make sure it's of that type
        if (type != m3u8::Tag::Tag_type::unknown) {
            Expects(type
                    == dynamic_cast<const m3u8::Tag*>(element.get())->type());
        }

        // We can now safely cast it
        return std::dynamic_pointer_cast<const T>(element);
    }

    /**
     * @brief Playlist base URI
     */
    const std::string& base_uri() const { return m_base_uri; }

private:
    /**
     * @brief See APlaylist_parser::stream
     *
     */
    m3u8::IElement_stream* m_stream;

    /**
     * @brief See APlaylist_parser::base_uri
     *
     */
    const std::string m_base_uri;
};

} // namespace playlist
} // namespace hls

#endif