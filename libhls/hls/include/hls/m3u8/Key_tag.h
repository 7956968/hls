#ifndef HLS_M3U8_KEYTAG_H_
#define HLS_M3U8_KEYTAG_H_

#include "hls/m3u8/Attribute_list.h"
#include "hls/m3u8/Tag.h"

#include <optional.hpp>

namespace hls {
namespace m3u8 {

/**
 * @brief Media Segments MAY be encrypted. The EXT-X-KEY tag specifies how to
 * decrypt them
 *
 * It applies to every Media Segment and to every Media Initialization Section
 * declared by an EXT-X-MAP tag that appears between it and the next EXT-X-KEY
 * tag in the Playlist file with the same KEYFORMAT attribute (or the end of
 * the Playlist file
 *
 * Two or more EXT-X-KEY tags with different KEYFORMAT attributes MAY apply to
 * the same Media Segment if they ultimately produce the same decryption key
 * [RFC8216]
 */
class Key_tag : public Tag {
public:
    /**
     * @brief Encryption method
     */
    enum class Method {
        /**
         * @brief An encryption method of NONE means that Media Segments are not
         * encrypted
         */
        none,

        /**
         * @brief An encryption method of AES-128 signals that Media Segments
         * are completely encrypted using the Advanced Encryption Standard
         * (AES) [AES_128] with a 128-bit key, Cipher Block Chaining (CBC),
         * and Public-Key Cryptography Standards #7 (PKCS7) padding [RFC5652].
         *
         * CBC is restarted on each segment boundary, using
         * either the Initialization Vector (IV) attribute value or the
         * Media Sequence Number as the IV [RFC8216]
         */
        aes_128,

        /**
         * An encryption method of SAMPLE-AES means that the Media Segments are
         * Sample Encrypted using the Advanced Encryption Standard [AES_128]
         * [RFC8216]
         */
        sample_aes
    };

public:
    /**
     * @brief Constructor
     */
    explicit Key_tag(const std::string& value,
                     const IVariable_resolver* variable_resolver);

    /**
     * @brief Encryption method. See Key_tag::Method
     */
    Method method() const;

    /**
     * @brief URI that specifies how to obtain the key.
     *
     * This attribute is REQUIRED unless the METHOD is NONE [RFC8216]
     */
    const nonstd::optional<std::string> uri() const;

    /**
     * @brief The value is a hexadecimal-sequence that specifies a 128-bit
     * unsigned integer Initialization Vector to be used with the key [RFC8216]
     */
    const nonstd::optional<std::string> iv() const;

    /**
     * @brief The value is a quoted-string that specifies how the key is
     * represented in the resource identified by the URI [RFC8216]
     */
    const nonstd::optional<std::string> key_format() const;

    /**
     * @brief The value is a quoted-string containing one or more positive
     * integers separated by the "/" character (for example, "1", "1/2", or
     * "1/2/5"
     *
     * If more than one version of a particular KEYFORMAT is defined, this
     * attribute can be used to indicate which version(s) this instance
     * complies with [RFC8216]
     */
    const nonstd::optional<std::string> key_format_versions() const;

private:
    /**
     * @brief See Key_tag::method
     *
     */
    Method m_method;

    /**
     * @brief See Key_tag::uri
     */
    nonstd::optional<std::string> m_uri;

    /**
     * @brief See Key_tag::iv
     */
    nonstd::optional<std::string> m_iv;

    /**
     * @brief See Key_tag::key_format
     */
    nonstd::optional<std::string> m_key_format;

    /**
     * @brief See Key_tag::key_format_versions
     */
    nonstd::optional<std::string> m_key_format_versions;

private:
    /**
     * @brief Method enum string parser
     */
    static std::function<Method(const std::string&)> s_method_parser;
};

} // namespace m3u8
} // namespace hls

#endif