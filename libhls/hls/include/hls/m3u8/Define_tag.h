#ifndef HLS_M3U8_DEFINETAG_H_
#define HLS_M3U8_DEFINETAG_H_

#include "hls/m3u8/Tag.h"

namespace hls {
namespace m3u8 {

/**
 * @brief Provides a playlist variable definition or declaration [RFC8216b]
 */
class Define_tag : public Tag {
public:
    /**
     * @brief Constructor
     */
    explicit Define_tag(const std::string& value,
                        const IVariable_resolver* variable_resolver);

    /**
     * @brief Variable name
     */
    nonstd::optional<std::string> name() const;

    /**
     * @brief Variable value
     */
    nonstd::optional<std::string> value() const;

    /**
     * @brief Import name. If it's defined, then ::name and ::value must not be
     * (instead of defining a variable, this tag is importing it from the master
     * list)
     */
    nonstd::optional<std::string> import_name() const;

private:
    /**
     * @brief See Define_tag::name
     */
    nonstd::optional<std::string> m_name;

    /**
     * @brief See Define_tag::value
     */
    nonstd::optional<std::string> m_value;

    /**
     * @brief See Define_tag::import
     */
    nonstd::optional<std::string> m_import_name;
};

} // namespace m3u8
} // namespace hls
#endif