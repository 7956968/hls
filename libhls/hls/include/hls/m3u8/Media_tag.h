#ifndef HLS_M3U8_Media_tag_H_
#define HLS_M3U8_Media_tag_H_

#include "hls/m3u8/Attribute_list.h"
#include "hls/m3u8/Tag.h"

#include <optional.hpp>

namespace hls {
namespace m3u8 {

/**
 * @brief The EXT-X-MEDIA tag is used to relate Media Playlists that contain
 * alternative Renditions (Section 4.3.4.2.1) of the same content. For example,
 * three EXT-X-MEDIA tags can be used to identify audio-only   Media Playlists
 * that contain English, French, and Spanish Renditions   of the same
 * presentation [RFC8216]
 */
class Media_tag : public Tag {
public:
    enum class Media_type { audio, video, subtitles, closed_captions };

    explicit Media_tag(const std::string& value);

    /**
     * @brief Media Media_type
     */
    Media_type media_type() const;

    /**
     * @brief The value is a quoted-string containing a URI that identifies the
     * Media Playlist file [RFC8216]
     */
    nonstd::optional<std::string> uri() const;

    /**
     * @brief The value is a quoted-string that specifies the group to which the
     * Rendition belongs [RFC8216]
     */
    const std::string& group_id() const;

    /**
     * @brief The value is a quoted-string containing one of the standard Tags
     * for Identifying Languages [RFC5646], which identifies the primary
     * language used in the Rendition [RFC8216]
     */
    nonstd::optional<std::string> language() const;

    /**
     * @brief The value is a quoted-string containing a language tag [RFC5646]
     * that identifies a language that is associated with the Rendition
     * [RFC8216]
     */
    nonstd::optional<std::string> assoc_language() const;

    /**
     * @brief The value is a quoted-string containing a human-readable
     * description of the Rendition [RFC8216]
     */
    const std::string& name() const;

    /**
     * @brief If the value is YES, then the client SHOULD play this Rendition of
     * the content in the absence of information from the user indicating a
     * different choice [RFC8216]
     */
    bool is_default() const;

    /**
     * @brief If the value is YES, then the client MAY choose to
     * play this Rendition in the absence of explicit user preference
     * because it matches the current playback environment, such as
     * chosen system language [RFC8216]
     */
    bool auto_select() const;

    /**
     * @brief A value of YES indicates that the Rendition contains content that
     * is considered essential to play [RFC8216]
     */
    bool forced() const;

    /**
     * @brief The value is a quoted-string that specifies a Rendition within the
     * segments in the Media Playlist [RFC8216]
     */
    nonstd::optional<std::string> in_stream_id() const;

    /**
     * @brief Contains one or more Uniform Media_type
     * Identifiers [UTI]. Each UTI indicates an individual
     * characteristic of the Rendition [RFC8216]
     */
    nonstd::optional<std::vector<std::string>> characteristics() const;

    /**
     * @brief If the Media_type attribute is AUDIO, then the first parameter is
     * a count of audio channels expressed as a decimal-integer, indicating the
     * maximum number of independent, simultaneous audio channels present in any
     * Media Segment in the Rendition
     *
     * If the Media_type attribute is AUDIO, then the second parameter
     * identifies the encoding of object-based audio used by the
     * Rendition. This parameter is a comma-separated list of Audio
     * Object Coding Identifiers  [RFC8216]
     */
    nonstd::optional<std::vector<std::string>> channels() const;

private:
    Media_type m_media_type;
    std::string m_group_id;
    std::string m_name;
    nonstd::optional<std::string> m_uri;
    nonstd::optional<std::string> m_language;
    nonstd::optional<std::string> m_assoc_language;
    bool m_default{false};
    bool m_auto_select{false};
    bool m_forced{false};
    nonstd::optional<std::string> m_in_stream_id;
    nonstd::optional<std::vector<std::string>> m_characteristics;
    nonstd::optional<std::vector<std::string>> m_channels;

private:
    static std::function<Media_type(const std::string&)> s_media_type_parser;
};

} // namespace m3u8
} // namespace hls

#endif
