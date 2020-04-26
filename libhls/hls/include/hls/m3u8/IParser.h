#ifndef HLS_M3U8_IPARSER_H_
#define HLS_M3U8_IPARSER_H_

#include "hls/m3u8/AElement.h"
#include "hls/m3u8/IVariable_repository.h"
#include "hls/m3u8/IVariable_resolver.h"

#include <memory>
#include <string>

namespace hls {
namespace m3u8 {

class IParser {
public:
    /**
     * @brief Parse a single line
     *
     * @param in_line Raw line
     */
    virtual std::unique_ptr<const AElement> parse_line(
      const std::string& line) = 0;

    /**
     * @brief Set the variable repository object
     *
     * @param repository
     */
    virtual void set_variable_repository(IVariable_repository* repository) = 0;

    /**
     * @brief Set the variable resolver object
     *
     * @param variable_resolver
     */
    virtual void set_variable_resolver(
      const IVariable_resolver* variable_resolver) = 0;

    virtual ~IParser() = default;
};

} // namespace m3u8
} // namespace hls

#endif