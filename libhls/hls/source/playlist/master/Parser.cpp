#include "hls/playlist/master/Parser.h"

namespace hls {
namespace playlist {
namespace master {

std::unique_ptr<Playlist> Parser::parse(m3u8::IElement_stream* stream,
                                        const std::string& uri) {
    Parser parser{stream, uri};
    return parser.parse();
}

Parser::Parser(m3u8::IElement_stream* stream, const std::string& uri)
    : APlaylist_parser(stream), m_base_uri{uri} {}

std::unique_ptr<Playlist> Parser::parse() {
    auto playlist{std::make_unique<Playlist>(m_base_uri)};

    bool header_found{false};

    while (true) {
        std::shared_ptr<const m3u8::AElement> element;

        try {
            element = stream()->get_next();
        } catch (const End_of_stream&) { break; }

        DILOGV("element: %s", to_string(element->type()).c_str());

        if (!header_found) {
            Expects(element->type() == m3u8::AElement::Type::tag,
                    Parse_error{"Unexpected tag before M3U: "s
                                + hls::to_string(element->type())});

            header_found = true;
            continue;
        }

        Expects(element->type() == m3u8::AElement::Type::tag);

        auto tag{dynamic_cast<const m3u8::Tag*>(element.get())};

        if (tag->type() == m3u8::Tag::Tag_type::x_stream_inf) {
            auto stream_inf{
              std::dynamic_pointer_cast<const m3u8::Stream_inf_tag>(element)};

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


    Expects(header_found, Parse_error{"Playlist header not found"});

    playlist->finalize();

    return playlist;
}


} // namespace master
} // namespace playlist
} // namespace hls
