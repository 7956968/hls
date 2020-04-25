#ifndef HLS_MANAGER_H_
#define HLS_MANAGER_H_

#include "hls/Common.h"

#include "hls/m3u8/Default_parser_factory.h"
#include "hls/m3u8/Parser_element_stream.h"
#include "hls/m3u8/String_line_reader.h"
#include "hls/playlist/master/Parser.h"
#include "hls/playlist/media/Parser.h"

#include "iwu/Default_downloader_factory.h"
#include "iwu/Log.h"
#include "iwu/Vector_downloader_destination.h"

#include <memory>
#include <mutex>
#include <string>

namespace hls {


class Segment_stream {
public:
    Segment_stream(std::unique_ptr<playlist::media::Playlist> playlist)
        : m_playlist{std::move(playlist)} {
        m_current_segment = m_playlist->segments().cbegin();
    }

    const playlist::media::Segment& get_next() {
        if (m_current_segment == m_playlist->segments().cend()) {
            throw End_of_stream{"No more segments available"};
        }

        const playlist::media::Segment& segment{*m_current_segment};

        ++m_current_segment;

        return segment;
    }

    const playlist::media::Playlist& playlist() const { return *m_playlist; }

    std::vector<playlist::media::Segment>::const_iterator m_current_segment;
    std::unique_ptr<const playlist::media::Playlist> m_playlist;
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
          open_playlist<playlist::master::Playlist, playlist::master::Parser>(
            uri);
    }

    /**
     * @brief Master playlist
     */
    const playlist::master::Playlist* master_playlist() const {
        std::lock_guard<std::mutex> lock{m_mutex};

        return m_master_playlist.get();
    }

    std::unique_ptr<Segment_stream> open_media_playlist(
      const hls::m3u8::Uri& uri) {
        return std::make_unique<Segment_stream>(
          open_playlist<playlist::media::Playlist, playlist::media::Parser>(
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

        return Parser_type::parse(parser_stream.get(), uri);
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
    std::unique_ptr<playlist::master::Playlist> m_master_playlist;
    std::unique_ptr<iwu::IDownloader> m_downloader;

    mutable std::mutex m_mutex;
}; // namespace hls

} // namespace hls

#endif