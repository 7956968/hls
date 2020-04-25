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
    APlaylist_parser(m3u8::IElement_stream* stream) : m_stream{stream} {}

    /**
     * @brief Element stream
     */
    m3u8::IElement_stream* stream() { return m_stream; }

    /**
     * @brief Read a tag and throw if next element is not tag of given type
     *
     * @param type (optional) Tag type to enforce
     */
    template<typename T>
    std::shared_ptr<const T>
    read_tag(m3u8::Tag::Tag_type type = m3u8::Tag::Tag_type::unknown) {
        // Read the tag
        std::shared_ptr<const m3u8::AElement> element{m_stream->get_next()};

        // Check that it's indeed a tag
        Expects(element->type() == m3u8::AElement::Type::tag,
                Error{"Expected tag, got "s + to_string(element->type())});

        // If type is specified, make sure it's of that type
        if (type != m3u8::Tag::Tag_type::unknown) {
            Expects(type
                      == dynamic_cast<const m3u8::Tag*>(element.get())->type(),
                    Error{"Unexpected tag type: expected="s + to_string(type)
                          + " got="s + to_string(element->type())});
        }

        // We can now safely cast it
        return std::dynamic_pointer_cast<const T>(element);
    }

    /**
     * @brief Read an element of given type, and throw if something else is read
     */
    template<typename T>
    std::shared_ptr<const T> read_type() {
        std::shared_ptr<const m3u8::AElement> element{m_stream->get_next()};

        auto typed_element{std::dynamic_pointer_cast<const T>(element)};
        Expects(typed_element, Error{"Unexpected element type "s
                                     + to_string(element->type())});

        return typed_element;
    }


private:
    /**
     * @brief See APlaylist_parser::stream
     *
     */
    m3u8::IElement_stream* m_stream;
};

} // namespace playlist
} // namespace hls

#endif