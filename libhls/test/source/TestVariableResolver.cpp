#include "Mock_repository.h"

#include "hls/Common.h"
#include "hls/m3u8/Variable_resolver.h"

#include "iwu/test/Test_common.h"

class TestVariableResolver : public ::testing::Test {};

TEST_F(TestVariableResolver, Resolve) {
    Mock_repository repo;

    hls::m3u8::Variable_resolver resolver{&repo};

    {
        repo.define_variable("first_var", "was");
        repo.define_variable("second_var", "ok !");
        std::string in{"Variable {$first_var} resolved {$second_var}"};

        ASSERT_EQ(resolver.resolve_variables(&in), 2);

        ASSERT_EQ(in, "Variable was resolved ok !"s);
    }
}

TEST_F(TestVariableResolver, UndefinedVariable) {
    Mock_repository repo;

    hls::m3u8::Variable_resolver resolver{&repo};

    {
        std::string in{"Variable {$undefined}"};

        ASSERT_ANY_THROW(resolver.resolve_variables(&in));
    }
}

TEST_F(TestVariableResolver, Malformed) {
    Mock_repository repo;

    hls::m3u8::Variable_resolver resolver{&repo};

    {
        std::string in{"Variable {$"};

        ASSERT_EQ(resolver.resolve_variables(&in), 0);
        ASSERT_EQ(in, "Variable {$");
    }
}

TEST_F(TestVariableResolver, Empty) {
    Mock_repository repo;

    hls::m3u8::Variable_resolver resolver{&repo};

    {
        std::string in{"{$}"};

        ASSERT_EQ(resolver.resolve_variables(&in), 0);

        ASSERT_EQ(in, "{$}"s);
    }

    {
        std::string in{""};

        ASSERT_EQ(resolver.resolve_variables(&in), 0);

        ASSERT_EQ(in, ""s);
    }
}
