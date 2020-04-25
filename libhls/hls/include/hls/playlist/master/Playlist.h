#ifndef HLS_M3U8MASTERPLAYLIST_H_
#define HLS_M3U8MASTERPLAYLIST_H_

#include "hls/m3u8/Media_tag.h"
#include "hls/m3u8/Stream_inf_tag.h"
#include "hls/m3u8/Uri.h"

#include "hls/playlist/APlaylist.h"
#include "hls/playlist/master/IRendition_group.h"
#include "hls/playlist/master/IVariant_stream.h"

#include <algorithm>

namespace hls {
namespace playlist {
namespace master {

/**
 * @brief A Playlist is a Master Playlist if all URI lines in the Playlist
 * identify Media Playlists
 *
 * A Playlist MUST be either a Media Playlist or a
 * Master Playlist; all other Playlists are invalid (source rfc8216)
 */
class Playlist : public APlaylist {
public:
    /**
     * Rendition group implementation. See IRendition_group for more info
     */
    struct Rendition_group : public IRendition_group {
        /**
         * @brief IRendition_group implementation
         */
    public:
        m3u8::Media_tag::Media_type type() const override { return media_type; }

        const std::string& id() const override { return group_id; }

        std::vector<const m3u8::Media_tag*> tags() const override {
            std::vector<const m3u8::Media_tag*> result;

            std::transform(
              media_tags.cbegin(), media_tags.cend(),
              std::back_inserter(result),
              [](const std::shared_ptr<const m3u8::Media_tag>& tag) {
                  return tag.get();
              });

            return result;
        }

    public:
        explicit Rendition_group(
          std::shared_ptr<const m3u8::Media_tag> first_tag)
            : media_type{first_tag->media_type()}, group_id{
                                                     first_tag->group_id()} {
            media_tags.push_back(first_tag);
        }

        /**
         * @brief See IRendition_group::type
         */
        m3u8::Media_tag::Media_type media_type;

        /**
         * @brief See IRendition_group::id
         */
        std::string group_id;

        /**
         * @brief See IRendition_group::media_tags
         */
        std::vector<std::shared_ptr<const m3u8::Media_tag>> media_tags;
    };

    /**
     * @brief See IVariant_stream
     */
    struct Variant_stream : public IVariant_stream {
        /**
         * @brief IVariant_stream implementation
         */
    public:
        const m3u8::Uri& uri() const override { return *uri_tag; }

        const m3u8::Stream_inf_tag& stream_inf() const override {
            return *stream_inf_tag;
        }

        const IRendition_group* get_rendition_group(
          m3u8::Media_tag::Media_type type) const override {
            auto group_iter{rendition_groups.find(type)};

            return group_iter != rendition_groups.end()
                     ? group_iter->second.get()
                     : nullptr;
        }

    public:
        Variant_stream(std::shared_ptr<const m3u8::Stream_inf_tag> stream_inf,
                       std::shared_ptr<const m3u8::Uri> uri)
            : stream_inf_tag{stream_inf}, uri_tag{uri} {}


        std::shared_ptr<const m3u8::Stream_inf_tag> stream_inf_tag;
        std::shared_ptr<const m3u8::Uri> uri_tag;
        std::map<m3u8::Media_tag::Media_type, std::shared_ptr<Rendition_group>>
          rendition_groups;
    };

public:
    explicit Playlist(const std::string& uri) : APlaylist{uri} {}

    void add_variant(std::shared_ptr<const m3u8::Stream_inf_tag> stream_inf,
                     std::shared_ptr<const m3u8::Uri> uri) {
        Expects(stream_inf);
        Expects(uri);

        m_variant_streams.push_back(
          std::make_shared<Variant_stream>(stream_inf, uri));
    }

    void add_media(std::shared_ptr<const m3u8::Media_tag> media_tag) {
        // Must have a name, group id and a type [RFC8216]
        Expects(!media_tag->group_id().empty());
        Expects(!media_tag->name().empty());
        // Expects(media_tag->media_type() != Media_tag::Media_type::not_set);

        auto group_iter{m_rendition_groups.find(media_tag->group_id())};
        if (group_iter != m_rendition_groups.end()) {
            // Check if a tag with this name is already in the group
            auto tag_iter{std::find_if(
              group_iter->second->media_tags.begin(),
              group_iter->second->media_tags.end(),
              [&media_tag](
                std::shared_ptr<const m3u8::Media_tag>& existing_tag) {
                  return existing_tag->name() == media_tag->name();
              })};

            Expects(tag_iter == group_iter->second->media_tags.end(),
                    Error{"Media tag with name '"s + media_tag->name() =
                            "' already added to rendition group"});

            // Insert it
            group_iter->second->media_tags.push_back(media_tag);
            return;
        }

        // Create a group and add this tag to it
        DILOGV("New rendition group: %s", media_tag->group_id().c_str());
        m_rendition_groups.insert(
          {media_tag->group_id(),
           std::make_shared<Rendition_group>(media_tag)});
    }

    void finalize() { connect_variant_streams_to_renditions(); }

    std::vector<const IVariant_stream*> streams() const {
        std::vector<const IVariant_stream*> result;

        std::transform(m_variant_streams.cbegin(), m_variant_streams.cend(),
                       std::back_inserter(result),
                       [](const std::shared_ptr<Variant_stream>& stream) {
                           return stream.get();
                       });

        return result;
    }

private:
    void connect_variant_streams_to_renditions() {
        const std::vector<m3u8::Media_tag::Media_type> types{
          m3u8::Media_tag::Media_type::audio,
          m3u8::Media_tag::Media_type::video,
          m3u8::Media_tag::Media_type::subtitles};

        for (auto& variant_stream : m_variant_streams) {
            for (auto media_type : types) {
                nonstd::optional<std::string> group_id{
                  variant_stream->stream_inf_tag->get_rendition_group_id(
                    media_type)};

                if (!group_id.has_value()) {
                    // No rendition group exists for this media type/variant
                    continue;
                }

                // Find our group (should exist at this point)
                std::shared_ptr<Rendition_group> rendition_group{
                  find_rendition_group(group_id.value(), media_type)};
                Expects(rendition_group);

                // Connect it to the variant stream
                variant_stream->rendition_groups[media_type] = rendition_group;
            }
        }
    }

    /**
     * @brief Find a rendition group by ID and type
     *
     * @param group_id Group ID
     * @param type Group type
     * @return group or nullptr if it doesn't exist
     */
    std::shared_ptr<Rendition_group> find_rendition_group(
      const std::string& group_id, m3u8::Media_tag::Media_type type) {
        auto group_iter{std::find_if(
          m_rendition_groups.begin(), m_rendition_groups.end(),
          [group_id,
           type](const std::pair<std::string, std::shared_ptr<Rendition_group>>&
                   element) {
              return element.second->group_id == group_id
                     && element.second->media_type == type;
          })};

        return group_iter != m_rendition_groups.end() ? group_iter->second
                                                      : nullptr;
    }

private:
    std::map<std::string, std::shared_ptr<Rendition_group>> m_rendition_groups;
    std::vector<std::shared_ptr<Variant_stream>> m_variant_streams;
};

} // namespace master
} // namespace playlist
} // namespace hls

#endif