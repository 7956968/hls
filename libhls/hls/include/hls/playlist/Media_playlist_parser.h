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

private:
    m3u8::IElement_stream* m_stream;
};


class Media_playlist_parser : public APlaylist_parser {
public:
    using APlaylist_parser::APlaylist_parser;

    std::unique_ptr<Media_playlist> parse() {
        auto playlist{std::make_unique<Media_playlist>()};


        bool header_found{false};


        std::vector<std::shared_ptr<const m3u8::AElement>> buffer;

        while (true) {
            if (!header_found) {
                read_tag<m3u8::Tag>(m3u8::Tag::Tag_type::m3u8);
                header_found = true;
                continue;
            }

            std::shared_ptr<const m3u8::AElement> element;

            try {
                element = stream()->get_next();
            } catch (const End_of_stream&) { break; }

            if (element->type() == m3u8::AElement::Type::uri) {
                buffer.clear();

                playlist->add_segment(Media_segment{
                  dynamic_cast<const m3u8::Uri*>(element.get())->uri()});
            }
        }

        return playlist;
    }

private:
    std::vector<std::shared_ptr<const m3u8::AElement>> m_buffer;
};

} // namespace playlist
} // namespace hls

#endif