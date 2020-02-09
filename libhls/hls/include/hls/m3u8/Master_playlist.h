#ifndef HLS_M3U8MASTERPLAYLIST_H_
#define HLS_M3U8MASTERPLAYLIST_H_


namespace hls {


namespace m3u8 {

/**
 * @brief  A Playlist MUST be either a Media Playlist or a Master Playlist;
 * all other Playlists are   invalid (source: rfc8216)
 */
class IPlaylist {
public:
    enum class Type { master, media, unknown };

    virtual Type type() const = 0;

    virtual ~IPlaylist() = default;
};

/**
 * @brief A Playlist is a Master Playlist if all URI   lines in the Playlist
 * identify Media Playlists.
 *
 * Master Playlist tags define the Variant Streams, Renditions, and   other
 * global parameters of the presentation.
 *
 *(source: rfc8216)
 *
 */
class Master_playlist : public IPlaylist {
public:
    Type type() const override { return Type::master; }
};


/**
 * @brief A Playlist is a Media Playlist if all URI lines in the Playlist
 * identify Media Segments (source: rfc8216)
 */
class Media_playlist {};
} // namespace m3u8
} // namespace hls

#endif