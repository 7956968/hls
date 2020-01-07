#ifndef HLS_COMMON_H_
#define HLS_COMMON_H_

#include "iwu/Log.h"
#include "iwu/backport/Language_support.h"

#include <gsl/gsl>
#undef Expects
#undef Ensures
#define IWU_CONTRACT_ALWAYS_THROW 1
#include "iwu/Contract.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wheader-hygiene"

/**
 * Literal namespace in header is an exception to the SF.7 rule
 */
using namespace std::literals;
using namespace iwu::backport::literals;

#pragma clang diagnostic pop


namespace hls {

enum class Status { OK, ERROR };

struct Error : public iwu::Error {
    using iwu::Error::Error;
};

} // namespace hls

#endif
