#ifndef HLS_MANAGER_H_
#define HLS_MANAGER_H_

#include "hls/Common.h"

#include "hls/m3u8/Default_parser_factory.h"
#include "hls/m3u8/Parser_element_stream.h"
#include "hls/m3u8/String_line_reader.h"
#include "hls/playlist/Master_playlist_parser.h"
#include "hls/playlist/Media_playlist_parser.h"

#include "iwu/Default_downloader_factory.h"
#include "iwu/Log.h"
#include "iwu/Vector_downloader_destination.h"

#include <memory>
#include <mutex>
#include <string>

namespace hls {


class Segment_stream {
public:
    Segment_stream(std::unique_ptr<playlist::Media_playlist> playlist)
        : m_playlist{std::move(playlist)} {
        m_current_segment = m_playlist->media_segments().cbegin();
    }

    const playlist::Media_segment& get_next() {
        if (m_current_segment == m_playlist->media_segments().cend()) {
            throw End_of_stream{"No more segments available"};
        }

        const playlist::Media_segment& segment{*m_current_segment};

        ++m_current_segment;

        return segment;
    }

    const playlist::Media_playlist& playlist() const { return *m_playlist; }

    std::vector<playlist::Media_segment>::const_iterator m_current_segment;
    std::unique_ptr<const playlist::Media_playlist> m_playlist;
};


class Manager {
public:
    Manager() : m_downloader{iwu::Default_downloader_factory{}.create()} {}

    /**
     * @brief Prepare the manager with the master playlist at given URI
     */
    void prepare(const std::string& uri) {
        std::lock_guard<std::mutex> lock{m_mutex};

        m_master_playlist =
          open_playlist<playlist::Master_playlist,
                        playlist::Master_playlist_parser>(uri);
    }

    /**
     * @brief Master playlist
     */
    const playlist::Master_playlist* master_playlist() const {
        std::lock_guard<std::mutex> lock{m_mutex};

        return m_master_playlist.get();
    }

    std::unique_ptr<Segment_stream> open_media_playlist(
      const hls::m3u8::Uri& uri) {
        return std::make_unique<Segment_stream>(
          open_playlist<playlist::Media_playlist,
                        playlist::Media_playlist_parser>(
            m_master_playlist->base_uri() + "/"s + uri.uri()));
    }

private:
    /**
     * @brief Open a playlist at given URI
     */
    template<typename Playlist_type, typename Parser_type>
    std::unique_ptr<Playlist_type> open_playlist(const std::string& uri) {
        std::unique_ptr<m3u8::IElement_stream> parser_stream{
          create_stream(uri)};

        return Parser_type{parser_stream.get()}.parse(uri);
    }

    /**
     * @brief Create an element stream at given URI
     */
    std::unique_ptr<m3u8::IElement_stream> create_stream(
      const std::string& uri) {
        iwu::Vector_downloader_destination buffer;
        iwu::http::Code code;

        const iwu::IDownloader::Download_status rc{m_downloader->start(
          iwu::IDownloader::Request_params{}.set_uri(uri).set_destination(
            &buffer),
          &code)};
        if (rc != iwu::IDownloader::Download_status::ok
            || code >= iwu::http::Code::error_start) {
            throw Error{"Playlist download failed: "s};
        }

        return std::make_unique<m3u8::Parser_element_stream>(
          std::make_unique<m3u8::String_line_reader>(
            buffer.get_buffer_as_string()),
          m3u8::Default_parser_factory{}.create());
    }

private:
    std::unique_ptr<playlist::Master_playlist> m_master_playlist;
    std::unique_ptr<iwu::IDownloader> m_downloader;

    mutable std::mutex m_mutex;
}; // namespace hls

} // namespace hls

#endif