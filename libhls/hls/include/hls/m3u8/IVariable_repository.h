#ifndef HLS_M3U8_VARIABLEREPOSITORY_H_
#define HLS_M3U8_VARIABLEREPOSITORY_H_

#include <string>

namespace hls {
namespace m3u8 {

/**
 * @brief Variable repository, used to store variables during parsing as well as
 * acquire their values
 */
class IVariable_repository {
public:
    /**
     * @brief Define a new variable
     *
     * @param name Variable name
     * @param value Variable value
     */
    virtual void define_variable(const std::string& name,
                                 const std::string& value) = 0;

    /**
     * @brief Request a variable to be imported into the repository
     *
     * @param name Variable name
     */
    virtual void import_variable(const std::string& name) = 0;

    /**
     * @brief Get the variable value
     *
     * @param name Variable name
     * @return Variable value
     */
    virtual std::string get_variable_value(const std::string& name) const = 0;

    /**
     * @brief Destructor
     */
    virtual ~IVariable_repository() = default;
};

} // namespace m3u8
} // namespace hls

#endif