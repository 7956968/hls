#ifndef HLS_PLAYLISTMASTERPLAYLISTPARSER_H_
#define HLS_PLAYLISTMASTERPLAYLISTPARSER_H_

#include "hls/Common.h"
#include "hls/m3u8/IElement_stream.h"
#include "hls/playlist/Master_playlist.h"

#include <vector>

namespace hls {
namespace playlist {

class Master_playlist_parser {
public:
    explicit Master_playlist_parser(m3u8::IElement_stream* stream)
        : m_stream{stream} {}

    std::unique_ptr<Master_playlist> parse(const std::string& uri) {
        auto playlist{std::make_unique<Master_playlist>(uri)};

        while (true) {
            std::shared_ptr<const m3u8::AElement> element;

            try {
                element = m_stream->get_next();
            } catch (const End_of_stream&) { break; }

            DILOGV("element: %s", to_string(element->type()).c_str());

            if (!m_header_found) {
                Expects(element->type() == m3u8::AElement::Type::tag,
                        Parse_error{"Unexpected tag before M3U: "s
                                    + hls::to_string(element->type())});

                m_header_found = true;
                continue;
            }

            Expects(element->type() == m3u8::AElement::Type::tag);

            auto tag{dynamic_cast<const m3u8::Tag*>(element.get())};

            if (tag->type() == m3u8::Tag::Tag_type::x_stream_inf) {
                auto stream_inf{
                  std::dynamic_pointer_cast<const m3u8::Stream_inf_tag>(
                    element)};

                // URI following the stream inf is required [RFC8216]
                auto uri{read_type<m3u8::Uri>()};

                DILOGV("new variant");
                playlist->add_variant(stream_inf, uri);
            } else if (tag->type() == m3u8::Tag::Tag_type::x_media) {
                auto media{
                  std::dynamic_pointer_cast<const m3u8::Media_tag>(element)};

                DILOGV("new media");
                playlist->add_media(media);

            } else {
                throw Parse_error{"Unexpected tag type: "s
                                  + hls::to_string(tag->type())};
            }
        }


        Expects(m_header_found, Parse_error{"Playlist header not found"});

        playlist->finalize();

        return playlist;
    }

    template<typename T>
    std::shared_ptr<const T> read_type() {
        std::shared_ptr<const m3u8::AElement> element{m_stream->get_next()};

        auto typed_element{std::dynamic_pointer_cast<const T>(element)};
        Expects(typed_element);

        return typed_element;
    }

private:
    std::vector<std::shared_ptr<const m3u8::AElement>> m_buffer;

    bool m_header_found{false};

    m3u8::IElement_stream* m_stream;
}; // namespace playlist

} // namespace playlist
} // namespace hls

#endif
