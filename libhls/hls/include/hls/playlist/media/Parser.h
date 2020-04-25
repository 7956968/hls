#ifndef HLS_PLAYLISTMEDIAPLAYLISTPARSER_H_
#define HLS_PLAYLISTMEDIAPLAYLISTPARSER_H_

#include "hls/Common.h"
#include "hls/m3u8/IElement_stream.h"
#include "hls/playlist/APlaylist_parser.h"
#include "hls/playlist/media/Playlist.h"

namespace hls {
namespace playlist {
namespace media {

class Parser : public APlaylist_parser {
public:
    using APlaylist_parser::APlaylist_parser;

    std::unique_ptr<Playlist> parse(const std::string& uri) {
        auto playlist{std::make_unique<Playlist>(uri)};

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
                playlist->add_segment(
                  Segment{dynamic_cast<const m3u8::Uri*>(element.get())->uri(),
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

} // namespace media
} // namespace playlist
} // namespace hls

#endif