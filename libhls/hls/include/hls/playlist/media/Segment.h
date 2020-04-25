#ifndef HLS_PLAYLISTMEDIASEGMENT_H_
#define HLS_PLAYLISTMEDIASEGMENT_H_

#include "hls/m3u8/Byte_range.h"
#include "hls/m3u8/Inf_tag.h"
#include "hls/m3u8/Key_tag.h"
#include "hls/m3u8/Map_tag.h"

namespace hls {
namespace playlist {
namespace media {

/**
 * @brief Media segment
 */
class Segment {
public:
    /**
     * @brief Constructor
     *
     * @param sequence_number See Segment::sequence_number
     * @param discontinuity_sequence_number See
     * Segment::discontinuity_sequence_number
     * @param uri See Segment::uri
     * @param inf_tag See Segment::inf_tag
     */
    explicit Segment(long sequence_number, long discontinuity_sequence_number,
                     const std::string& uri,
                     std::shared_ptr<const m3u8::Inf_tag> inf_tag)
        : m_sequence_number{sequence_number},
          m_discontinuity_sequence_number{discontinuity_sequence_number},
          m_uri{uri} {
        Expects(!uri.empty(), Error{"URI missing"s});

        Expects(inf_tag, Error{"Mandatory inf tag missing"s});

        m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::duration<double>(inf_tag->duration()));
        m_title = inf_tag->title();
    }

    /**
     * @brief URI which can be used to download the segment
     */
    const std::string& uri() const { return m_uri; }

    /**
     * @brief Unique media sequence number
     */
    long sequence_number() const { return m_sequence_number; }

    /**
     * @brief Count of how many times a discontinuity happened before this
     * segment
     */
    long discontinuity_sequence_number() const {
        return m_discontinuity_sequence_number;
    }

    /**
     * @brief Segment time duration
     */
    std::chrono::nanoseconds duration() const { return m_duration; }

    /**
     * @brief Optional segment human readable title
     */
    nonstd::optional<std::string> title() const { return m_title; }

    /**
     * @brief Byte range with which the URI should be downloaded
     */
    nonstd::optional<m3u8::Byte_range> byte_range() const {
        return m_byte_range;
    }

    /**
     * @brief Indicates that the segment URL to which it applies does not
     * contain media data and SHOULD NOT be loaded by clients
     */
    bool is_gap() const { return m_gap; }

    /**
     * @brief Absolute date and/or time of the first sample of a segment
     */
    std::shared_ptr<const m3u8::Tag> program_date_time() const {
        return m_program_date_time;
    }

    /**
     * @brief Specifies how to decrypt segments, if encrypted
     */
    std::shared_ptr<const m3u8::Key_tag> key() const { return m_key; }

    /**
     * @brief Specifices how to obtain the Media Initialization Section required
     * to parse the media segment
     */
    std::shared_ptr<const m3u8::Map_tag> map() const { return m_map; }

    /**
     * @brief Approximate bit rate of the segment
     */
    nonstd::optional<long> bitrate() const { return m_bitrate; }

    Segment& set_byte_range(const m3u8::Byte_range& byte_range) {
        m_byte_range = byte_range;
        return *this;
    }

    Segment& set_program_date_time(
      std::shared_ptr<const m3u8::Tag> program_date_time) {
        m_program_date_time = program_date_time;
        return *this;
    }

    Segment& set_gap(bool gap) {
        m_gap = gap;
        return *this;
    }

    Segment& set_key(std::shared_ptr<const m3u8::Key_tag> key) {
        m_key = key;
        return *this;
    }

    Segment& set_map(std::shared_ptr<const m3u8::Map_tag> map) {
        m_map = map;
        return *this;
    }
    Segment& set_bitrate(long bitrate) {
        m_bitrate = bitrate;
        return *this;
    }

private:
    /**
     * @brief See Segment::sequence_number
     */
    long m_sequence_number;

    /**
     * @brief See Segment::discontinuity_sequence_number
     */
    long m_discontinuity_sequence_number;

    /**
     * @brief See Segment::uri
     */
    std::string m_uri;

    /**
     * @brief See Segment::byte_range
     */
    nonstd::optional<m3u8::Byte_range> m_byte_range;

    /**
     * @brief See Segment::duration
     */
    std::chrono::nanoseconds m_duration;

    /**
     * @brief See Segment::title
     */
    nonstd::optional<std::string> m_title;

    /**
     * @brief See Segment::program_date_time
     */
    std::shared_ptr<const m3u8::Tag> m_program_date_time;

    /**
     * @brief See Segment::is_gap
     */
    bool m_gap{false};

    /**
     * @brief See Segment::key
     */
    std::shared_ptr<const m3u8::Key_tag> m_key;

    /**
     * @brief See Segment::map
     */
    std::shared_ptr<const m3u8::Map_tag> m_map;

    /**
     * @brief See Segment::bitrate
     */
    nonstd::optional<long> m_bitrate;
};

} // namespace media
} // namespace playlist
} // namespace hls

#endif