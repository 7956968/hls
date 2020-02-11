#ifndef HLS_M3U8_STREAMINFTAG_H_
#define HLS_M3U8_STREAMINFTAG_H_

#include "hls/m3u8/Attribute_list.h"
#include "hls/m3u8/Media_tag.h"
#include "hls/m3u8/Tag.h"

namespace hls {
namespace m3u8 {

/**
 * @brief The EXT-X-STREAM-INF tag specifies a Variant Stream, which is a set of
 * Renditions that can be combined to play the presentation.  The   attributes
 * of the tag provide information about the Variant Stream. The URI line that
 * follows the EXT-X-STREAM-INF tag specifies a Media   Playlist that carries a
 * Rendition of the Variant Stream. (source rfc8216)
 *
 */
class Stream_inf_tag : public Tag {
public:
    enum class Hdcp_level { type_0, type_1, none };

    enum class Video_range { sdr, pq };

public:
    Stream_inf_tag(const std::string& value);

    /**
     * @brief The value is a decimal-integer of bits per second. It represents
     the peak segment bit rate of the Variant Stream. (source rfc8216).
     */
    long bandwidth() const;

    /**
     * The value is a decimal-integer of bits per second. It represents
     * the average segment bit rate of the Variant Stream. (source rfc8216)
     */
    nonstd::optional<long> average_bandwidth() const;

    /**
     * @brief A list of formats, where  where each format specifies a media
     * sample type that is present in one or more Renditions specified by the
     * Variant Stream. Valid format identifiers are those in the ISO Base Media
     * File Format Name Space (source rfc8216)
     */
    nonstd::optional<std::vector<std::string>> codecs() const;

    /**
     * @brief The value is a decimal-resolution describing the optimal pixel
     * resolution at which to display all the video in the Variant Stream
     * (source rfc8216)
     */
    nonstd::optional<Attribute_list::Resolution> resolution() const;

    /**
     * @brief The value is a decimal-floating-point describing the maximum frame
     * rate for all the video in the Variant Stream, rounded to three
     * decimal places (source rfc8216)
     *
     * @return frame rate if present, -1.0 otherwise
     */
    nonstd::optional<float> frame_rate() const;

    /**
     * @brief Minimum HDCP level required to play encrypted stream. Clients
     * without output copy protection SHOULD NOT load a Variant Stream with an
     * HDCP-LEVEL attribute unless its value is NONE (source rfc8216)
     */
    nonstd::optional<Hdcp_level> hdcp_level() const;

    /**
     * @brief The ALLOWED-CPC attribute allows a server to indicate that the
     * playback of a Variant Stream containing encrypted Media Segments
     * is to be restricted to devices that guarantee a certain level of
     * content protection robustness (source rfc8216)
     */
    nonstd::optional<std::vector<std::string>> allowed_cpc() const;

    /**
     * @brief The value MUST be SDR if all the video in the Variant Stream is
     * encoded using a reference opto-electronic transfer characteristic
     * function specified by the TransferCharacteristics code point 1 [CICP].
     * The value MUST be PQ if video in the Variant Stream includes some
     * content that is encoded using a reference opto-electronic transfer
     * characteristic function specified by the TransferCharacteristics code
     * point 16 or 18 (source rfc8216)
     */
    nonstd::optional<Video_range> video_range() const;

    /**
     * @brief It MUST match the value of the
     * GROUP-ID attribute of an EXT-X-MEDIA tag elsewhere in the Master
     * Playlist whose TYPE attribute is AUDIO. It indicates the set of
     * audio Renditions that SHOULD be used when playing the
     * presentation (source rfc8216)
     */
    nonstd::optional<std::string> audio() const;

    /**
     * @brief It MUST match the value of the
     * GROUP-ID attribute of an EXT-X-MEDIA tag elsewhere in the Master
     * Playlist whose TYPE attribute is VIDEO. It indicates the set of
     * video Renditions that SHOULD be used when playing the
     * presentation (source rfc8216)
     */
    nonstd::optional<std::string> video() const;

    /**
     * @brief The value is a quoted-string. It MUST match the value of the
     * GROUP-ID attribute of an EXT-X-MEDIA tag elsewhere in the Master
     * Playlist whose TYPE attribute is SUBTITLES. It indicates the set
     * of subtitle Renditions that can be used when playing the
     * presentation (source rfc8216)
     */
    nonstd::optional<std::string> subtitles() const;

    nonstd::optional<std::string> get_rendition_group_id(
      Media_tag::Media_type type) const;

    /**
     * @brief The value can be either a quoted-string or an enumerated-string
     * with the value NONE. If the value is a quoted-string, it MUST
     * match the value of the GROUP-ID attribute of an EXT-X-MEDIA tag
     * elsewhere in the Playlist whose TYPE attribute is CLOSED-CAPTIONS,
     * and it indicates the set of closed-caption Renditions that can be
     * used when playing the presentation.
     */
    nonstd::optional<std::pair<bool, std::string>> closed_captions() const;

private:
    long m_bandwidth{-1};
    nonstd::optional<long> m_average_bandwidth;
    nonstd::optional<std::vector<std::string>> m_codecs;
    nonstd::optional<Attribute_list::Resolution> m_resolution;
    nonstd::optional<float> m_frame_rate;
    nonstd::optional<Hdcp_level> m_hdcp_level;
    nonstd::optional<std::vector<std::string>> m_allowed_cpc;
    nonstd::optional<Video_range> m_video_range;
    nonstd::optional<std::string> m_audio;
    nonstd::optional<std::string> m_video;
    nonstd::optional<std::string> m_subtitles;
    nonstd::optional<std::pair<bool, std::string>> m_closed_captions;

private:
    static std::function<Hdcp_level(const std::string&)> s_hdcp_level_parser;
    static std::function<Video_range(const std::string&)> s_video_range_parser;
}; // namespace m3u8

} // namespace m3u8
} // namespace hls

#endif
