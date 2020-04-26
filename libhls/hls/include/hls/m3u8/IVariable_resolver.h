#ifndef HLS_M3U8_IVARIABLERESOLVER_H_
#define HLS_M3U8_IVARIABLERESOLVER_H_

#include <string>

#include "hls/Common.h"

namespace hls {
namespace m3u8 {

/**
 * @brief Variable resolver in charge of replacing variable names with their
 * values
 */
class IVariable_resolver {
public:
    /**
     * @brief Resolve variables in-place
     *
     * @param input(inout) Input string
     * @return Number of variables replaced
     */
    virtual gsl::index resolve_variables(std::string* input) const = 0;

    /**
     * @brief Destructor
     */
    virtual ~IVariable_resolver() = default;
};

} // namespace m3u8
} // namespace hls

#endif