#include "hls/m3u8/Variable_resolver.h"

#include <regex>

namespace hls {
namespace m3u8 {

Variable_resolver::Variable_resolver(const IVariable_repository* repository)
    : m_repository{repository}, m_regex{// Opening bracket
                                        "\\{"
                                        // Variable name prefix
                                        "\\$"
                                        // Variable name
                                        "([^\\}]+)"
                                        // Closing bracket
                                        "\\}"

                                } {}

gsl::index Variable_resolver::resolve_variables(std::string* input) const {
    gsl::index num_replacements{0};

    while (true) {
        // Search for the next variable
        std::smatch match;
        if (!std::regex_search(*input, match, m_regex)) {
            return num_replacements;
        }

        // Sanity check
        Expects(match.size() == 2);

        // Extract name
        const std::string& variable_name{match[1].str()};

        // Fetch value from the repo
        const std::string variable_value{
          m_repository->get_variable_value(variable_name)};

        // Replace name with value
        input->replace(match[0].first, match[0].second, variable_value);

        ++num_replacements;
    }
}


} // namespace m3u8
} // namespace hls
