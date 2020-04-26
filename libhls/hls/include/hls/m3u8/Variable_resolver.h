#ifndef HLS_M3U8_VARIABLERESOLVER_H_
#define HLS_M3U8_VARIABLERESOLVER_H_

#include "hls/Common.h"
#include "hls/m3u8/IVariable_repository.h"
#include "hls/m3u8/IVariable_resolver.h"

#include <regex>

namespace hls {
namespace m3u8 {

/**
 * @brief Variable resolver implementation. See IVariable_resolver
 */
class Variable_resolver : public IVariable_resolver {
public:
    /**
     * @brief Constructor
     *
     * @param repository Variable repository the resolver will fetch variable
     * values from
     */
    Variable_resolver(const IVariable_repository* repository);

    /**
     * @brief IVariable_resolver implementation
     */
public:
    gsl::index resolve_variables(std::string* input) const override;

private:
    /**
     * @brief Variable repository
     */
    const IVariable_repository* m_repository;

    /**
     * @brief Regex used to parse the input with
     */
    std::regex m_regex;
};

} // namespace m3u8
} // namespace hls

#endif