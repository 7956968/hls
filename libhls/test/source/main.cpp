#include "gtest/gtest.h"
#include <iwu/Log.h>


int main(int argc, char **argv) {

    // Enable library logging
    iwu::Log::setLevel(iwu::Log::eLEVEL_VERBOSE);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
