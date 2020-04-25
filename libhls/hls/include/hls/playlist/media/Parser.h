#ifndef HLS_PLAYLISTMEDIAPLAYLISTPARSER_H_
#define HLS_PLAYLISTMEDIAPLAYLISTPARSER_H_

#include "hls/Common.h"
#include "hls/m3u8/Byte_range_tag.h"
#include "hls/m3u8/Common.h"
#include "hls/m3u8/Enum_tag.h"
#include "hls/m3u8/IElement_stream.h"
#include "hls/m3u8/Integer_tag.h"
#include "hls/m3u8/Key_tag.h"
#include "hls/m3u8/Map_tag.h"
#include "hls/m3u8/Start_tag.h"
#include "hls/playlist/APlaylist_parser.h"
#include "hls/playlist/media/Playlist.h"

namespace hls {
namespace playlist {
namespace media {

class Parser : public APlaylist_parser {
public:
    /**
     * @brief Parse the playlist
     *
     * @param stream Element stream
     * @param uri Playlist URI
     * @return Parsed playlist
     */
    static std::unique_ptr<Playlist> parse(m3u8::IElement_stream* stream,
                                           const std::string& uri);

private:
    /**
     * @brief Constructor
     *
     * @param stream Element stream
     * @param base_uri Playlist URI
     */
    Parser(m3u8::IElement_stream* stream, const std::string& base_uri);

    /**
     * @brief Parse the playlist
     *
     * @return Parsed playlist
     */
    std::unique_ptr<Playlist> parse();

private:
    /**
     * @brief Parse next element from the stream. This may result in some
     * additional playlist metadata or a new segment
     *
     * @throw End_of_stream when all elements are depleted
     */
    void parse_next();

private:
    /**
     * @brief Segment specific tags that only apply to the next segment
     */
    struct Segment_tags {
        /**
         * @brief #EXT-INF tag (mandatory)
         */
        std::shared_ptr<const m3u8::Inf_tag> inf;

        /**
         * @brief #EXT-X-BYTERANGE tag (optional)
         */
        std::shared_ptr<const m3u8::Byte_range_tag> byte_range;

        /**
         * @brief #EXT-X-PROGRAM-DATE-TIME tag (optional)
         */
        std::shared_ptr<const m3u8::Tag> program_date_time;

        /**
         * @brief Indication that we ran into an #EXT-X-GAP tag
         */
        bool gap{false};
    };

    /**
     * @brief Tags that are global to the media playlist
     */
    struct Playlist_tags {
        /**
         * @brief #EXTM3U tag (mandatory)
         */
        std::shared_ptr<const m3u8::Tag> m3u;

        /**
         * @brief #EXT-X-TARGET_DURATION tag (mandatory)
         */
        std::shared_ptr<const m3u8::Integer_tag> target_duration;

        /**
         * @brief Playlist type extracted from #EXT-X-PLAYLIST-TYPE tag
         * (optional)
         */
        nonstd::optional<m3u8::Playlist_type> playlist_type;

        /**
         * @brief Indicates that we ran into an #EXT-X-I-FRAMES-ONLY tag
         * (optional)
         */
        bool iframes_only{false};

        /**
         * @brief Indicates that we ran into an #EXT-X-INDEPENDENT-SEGMENTS tag
         * (optional)
         */
        bool independent_segments{false};

        /**
         * @brief #EXT-X-START tag (optional)
         */
        std::shared_ptr<const m3u8::Start_tag> start;
    };

    /**
     * @brief Shared tags that should be applied to all the
     * segments which. May be overwritten by new values as we go follow it
     */
    struct Segment_shared_tags {
        /**
         * @brief #EXT-X-KEY tag (optional)
         */
        std::shared_ptr<const m3u8::Key_tag> key;

        /**
         * @brief #EXT-X-MAP tag (optional)
         */
        std::shared_ptr<const m3u8::Map_tag> map;

        /**
         * @brief #-X-BITRATE tag (optional)
         */
        std::shared_ptr<const m3u8::Integer_tag> bitrate;
    };

private:
    /**
     * @brief Playlist base URI
     */
    std::string m_base_uri;

    /**
     * @brief Global playlist tags
     */
    Playlist_tags m_playlist_tags;

    /**
     * @brief Segment specific tags
     */
    Segment_tags m_segment_tags;

    /**
     * @brief Tags shared between all following segments
     */
    Segment_shared_tags m_segment_shared_tags;

    /**
     * @brief Defaulted to 0, may be overriden by EXT-X-MEDIASEQUENCE tag.
     * Increases every time a segment is created
     */
    long m_media_sequence_number{0};

    /**
     * @brief Defaulted to 0. Overriden by EXT-X-DISCONTINUITY-SEQUENCE
     * tag. Increased by 1 every time EXT-X-DISCONTINUITY is encountered
     */
    long m_discontinuity_sequence_number{0};

    /**
     * @brief Indication if at least one segment was read (i.e. header is done)
     */
    bool m_header_read{false};

    /**
     * @brief Playlist we created
     */
    std::unique_ptr<Playlist> m_playlist;

    /**
     * @brief List of segments we created
     */
    std::vector<Segment> m_segments;
};

} // namespace media
} // namespace playlist
} // namespace hls

#endif