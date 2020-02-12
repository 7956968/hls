#ifndef HLS_PLAYLISTMEDIAPLAYLISTPARSER_H_
#define HLS_PLAYLISTMEDIAPLAYLISTPARSER_H_

#include "hls/Common.h"
#include "hls/m3u8/IElement_stream.h"
#include "hls/playlist/Master_playlist.h"
#include "hls/playlist/Media_playlist.h"


namespace hls {
namespace playlist {

class APlaylist_parser {
public:
    APlaylist_parser(m3u8::IElement_stream* stream) : m_stream{stream} {}


    m3u8::IElement_stream* stream() { return m_stream; }


    template<typename T>
    std::shared_ptr<const T>
    read_tag(m3u8::Tag::Tag_type type = m3u8::Tag::Tag_type::unknown) {
        std::shared_ptr<const m3u8::AElement> element{m_stream->get_next()};

        Expects(element->type() == m3u8::AElement::Type::tag);

        if (type != m3u8::Tag::Tag_type::unknown) {
            Expects(type
                    == dynamic_cast<const m3u8::Tag*>(element.get())->type());
        }

        return std::dynamic_pointer_cast<const T>(element);
    }

    const std::string& base_uri() const { return m_base_uri; }

private:
    m3u8::IElement_stream* m_stream;
    const std::string m_base_uri;
};


class Media_playlist_parser : public APlaylist_parser {
public:
    using APlaylist_parser::APlaylist_parser;

    std::unique_ptr<Media_playlist> parse(const std::string& uri) {
        auto playlist{std::make_unique<Media_playlist>(uri)};

        // Indication that the playlist header was found
        bool header_found{false};

        // Elements buffered before we ran into an URI (terminating element for
        // a segment)
        std::vector<std::shared_ptr<const m3u8::AElement>> buffer;

        while (true) {
            // Read the header
            if (!header_found) {
                read_tag<m3u8::Tag>(m3u8::Tag::Tag_type::m3u8);
                header_found = true;
                continue;
            }

            // Read next element
            std::shared_ptr<const m3u8::AElement> element;
            try {
                element = stream()->get_next();
            } catch (const End_of_stream&) { break; }

            // Ran into an uri, so we can construct a segment
            if (element->type() == m3u8::AElement::Type::uri) {
                std::shared_ptr<const m3u8::Inf_tag> inf_tag;

                // Check all the gathered elements for segment metadata
                for (auto& buffered_element : buffer) {
                    // Only expecting tags here
                    Expects(buffered_element->type()
                            == m3u8::AElement::Type::tag);
                    auto tag{
                      dynamic_cast<const m3u8::Tag*>(buffered_element.get())};

                    if (tag->type() == hls::m3u8::Tag::Tag_type::inf) {
                        inf_tag =
                          std::dynamic_pointer_cast<const m3u8::Inf_tag>(
                            buffered_element);
                    } else {
                        DILOGW("Unhandled tag: %s",
                               to_string(tag->type()).c_str());
                    }
                }

                buffer.clear();

                // Add a segment
                playlist->add_segment(Media_segment{
                  dynamic_cast<const m3u8::Uri*>(element.get())->uri(),
                  inf_tag});
            } else {
                // Just buffer the elements and process later
                buffer.push_back(element);
            }
        }

        Ensures(header_found);

        return playlist;
    }

private:
    std::vector<std::shared_ptr<const m3u8::AElement>> m_buffer;
};

} // namespace playlist
} // namespace hls

#endif