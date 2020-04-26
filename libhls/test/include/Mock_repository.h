#ifndef HLS_TEST_MOCKREPOSITORY_H_
#define HLS_TEST_MOCKREPOSITORY_H_

#include "hls/Common.h"
#include "hls/m3u8/IVariable_repository.h"
#include "iwu/test/Test_common.h"

#include <map>

namespace {

struct Mock_repository : public hls::m3u8::IVariable_repository {
    void define_variable(const std::string& name,
                         const std::string& value) override {
        TRACE_CALL("name='%s', value='%s'", name.c_str(), value.c_str());
        m_variables.insert({name, value});
    }

    void import_variable(const std::string& name) override {
        TRACE_CALL("name='%s'", name.c_str());

        auto import_iter{m_imports.find(name)};
        if (import_iter == m_imports.end()) {
            throw hls::Error{"Import not defined"};
        }

        // Move to variables
        m_variables.insert({import_iter->first, import_iter->second});
    }

    std::string get_variable_value(const std::string& name) const override {
        TRACE_CALL("name='%s'", name.c_str());

        auto var_iter{m_variables.find(name)};

        if (var_iter == m_variables.end()) {
            throw hls::Error{"Variable not defined"};
        }

        return var_iter->second;
    }


    std::map<std::string, std::string> m_variables;
    std::map<std::string, std::string> m_imports;
};

} // namespace

#endif