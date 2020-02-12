#include "hls/Manager.h"

#include "iwu/Command_executor.h"
#include "iwu/Log.h"

#include <fstream>

/**
 * @brief

https://developer.apple.com/streaming/examples/

 Advanced TS
 https://devstreaming-cdn.apple.com/videos/streaming/examples/img_bipbop_adv_example_ts/master.m3u8

 Advanced fMP4
 https://devstreaming-cdn.apple.com/videos/streaming/examples/img_bipbop_adv_example_fmp4/master.m3u8
 https://devstreaming-cdn.apple.com/videos/streaming/examples/img_bipbop_adv_example_fmp4/v5/prog_index.m3u8
 Advanced (HEVC/H.264)
 https://devstreaming-cdn.apple.com/videos/streaming/examples/bipbop_adv_example_hevc/master.m3u8

Basic
https://devstreaming-cdn.apple.com/videos/streaming/examples/bipbop_4x3/bipbop_4x3_variant.m3u8

Basic
https://devstreaming-cdn.apple.com/videos/streaming/examples/bipbop_16x9/bipbop_16x9_variant.m3u8

 *
 */

class Cli_app : public iwu::Command_executor {
public:
    Cli_app() {
        m_command_executor.register_command(
          "prepare",
          std::bind(&Cli_app::command_prepare, this, std::placeholders::_1));

        m_command_executor.register_command(
          "ls", std::bind(&Cli_app::command_ls, this, std::placeholders::_1));

        m_command_executor.register_command(
          "quit",
          std::bind(&Cli_app::command_quit, this, std::placeholders::_1));

        {
            m_command_executor.execute_string(
              "prepare "
              "https://bitdash-a.akamaihd.net/content/sintel/hls/"
              "playlist.m3u8");

            auto stream{m_manager.open_media_playlist(
              m_manager.master_playlist()->streams()[0]->uri())};

            iwu::Vector_downloader_destination dest;
            std::unique_ptr<iwu::IDownloader> downloader{
              iwu::Default_downloader_factory{}.create()};

            std::ofstream output{"dump.ts"};

            try {
                while (true) {
                    const hls::playlist::Media_segment& segment{
                      stream->get_next()};

                    const std::string segment_uri{stream->playlist().base_uri()
                                                  + "/"s + segment.uri()};
                    DILOGV("Download: %s", segment_uri.c_str());

                    iwu::http::Code code;
                    const iwu::IDownloader::Download_status rc{
                      downloader->start(iwu::IDownloader::Request_params{}
                                          .set_destination(&dest)
                                          .set_uri(segment_uri),
                                        &code)};
                    Expects(rc == iwu::IDownloader::Download_status::ok);
                    Expects(code < iwu::http::Code::error_start);
                    DILOGV("\t downloaded %lld bytes",
                           downloader->get_bytes_downloaded());

                    output.write(
                      reinterpret_cast<const char*>(dest.get_buffer().data()),
                      dest.get_buffer().size());
                }
            } catch (hls::End_of_stream&) {}

            m_command_executor.execute_string("ls");
        }
    }

    iwu::Status command_ls(const iwu::Command_executor::Command&) {
        m_manager.master_playlist();

        std::vector<const hls::playlist::IVariant_stream*> streams{
          m_manager.master_playlist()->streams()};

        DILOGI("Variant streams: %d", streams.size());

        for (auto stream : streams) {
            DILOGI("Stream: %s [ %d bps ]", stream->uri().uri().c_str(),
                   stream->stream_inf().bandwidth());

            const std::vector<hls::m3u8::Media_tag::Media_type> media_types{
              {hls::m3u8::Media_tag::Media_type::audio,
               hls::m3u8::Media_tag::Media_type::video,
               hls::m3u8::Media_tag::Media_type::subtitles}};

            for (auto type : media_types) {
                const hls::playlist::IRendition_group* group{
                  stream->get_rendition_group(type)};
                if (!group) {
                    continue;
                }

                DILOGI("\t%s groups: %d", hls::to_string(type).c_str(),
                       group->tags().size());
            }
        }

        return iwu::Status::ok;
    }

    iwu::Status command_quit(const iwu::Command_executor::Command&) {
        m_running = false;
        return iwu::Status::ok;
    }

    iwu::Status command_prepare(const iwu::Command_executor::Command& cmd) {
        if (cmd.args.size() != 1) {
            DILOGE("prepare <uri>");
            return iwu::Status::error;
        }

        const std::string& uri{cmd.args.front()};

        DILOGI("perparing: '%s' ..", uri.c_str());

        m_manager.prepare(uri);

        DILOGI("Stream prepare");

        return iwu::Status::ok;
    }

    void run() {
        while (m_running) {
            m_command_executor.execute_one();
        }
    }

private:
    iwu::Command_executor m_command_executor;

    bool m_running{true};

    hls::Manager m_manager;
};

int main() {
    // Enable library logging
    iwu::Log::set_level(iwu::Log::Level::verbose);
    DLOGE("@# wot wot");

    Cli_app{}.run();

    return 0;
}