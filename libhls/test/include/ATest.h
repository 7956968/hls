#ifndef IWU_TEST_TEST_H_
#define IWU_TEST_TEST_H_

#include <gtest/gtest.h>
#include <string>
#include <iwu/Log.h>

#define MULTI_LINE_STRING(name, content) \
    static const std::string& name = #content \
    ;


#define GROUP_TEST_F(GroupName, TestBase, TestName) \
   TEST_F(TestBase, GroupName##_##TestName) \

class ATest: public virtual ::testing::Test {
public:
    virtual ~ATest() {
    }

protected:
    std::string getAssetPath(const std::string& name) const;

    bool loadFile(const std::string& name, std::vector<uint8_t>& buffer) const;
};

#endif
