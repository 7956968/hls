#include "iwu/test/Test_common.h"

int main(int argc, const char** argv) {
    // Enable logging
    iwu::Log::set_level(iwu::Log::Level::verbose);

    return iwu::test::test_main(gsl::span<const char*>{argv, argv + argc});
}
