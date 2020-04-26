#ifndef HLS_M3U8ATTRIBUTELIST_H_
#define HLS_M3U8ATTRIBUTELIST_H_

#include "hls/Common.h"
#include "hls/m3u8/IVariable_resolver.h"

#include <regex>
#include <unordered_map>
#include <variant.hpp>

namespace hls {
namespace m3u8 {

/**
 * @brief Attribute list container  which models Attribute Lists specified in
 * RFC8216b/4.2
 */
class Attribute_list {
public:
    /**
     * @brief Helper boolean enum, used to parse YES/NO attribute values
     */
    enum class Bool_enum { yes, no };

    /**
     * @brief Resolution type (width/height)
     */
    using Resolution = std::pair<long, long>;

    /**
     * @brief String enumeration
     *
     */
    struct String_enum {
        std::string value;

        explicit String_enum(const std::string& value) : value{value} {}
    };

    /**
     * @brief Field variant
     */
    using Variant_t =
      mpark::variant<long long, float, std::string, Resolution, String_enum>;

    using Container_t = std::unordered_map<std::string, Variant_t>;

    static constexpr gsl::index k_variant_number_integer_i{0};
    static constexpr gsl::index k_variant_number_float_i{1};
    static constexpr gsl::index k_variant_string_i{2};
    static constexpr gsl::index k_variant_resolution_i{3};
    static constexpr gsl::index k_variant_enum_string_i{4};

    /**
     * @brief Error which indicates that a field with this name doesn't exist
     *
     */
    struct No_such_field_error : public Error {
        using Error::Error;
    };

public:
    /**
     * @brief Constructor
     *
     * @param input Raw input string
     * @param variable_resolver Used to resolve any possible variables in quoted
     * string fields
     */
    explicit Attribute_list(const std::string& input,
                            const IVariable_resolver* variable_resolver);

    /**
     * @brief Get a field
     *
     * @param name Field name
     * @return Variant object
     */
    Variant_t get(const std::string& name) const;

    /**
     * @brief Get a field of given type
     *
     * @tparam T Field type
     * @param name  Field name
     * @return field value
     */
    template<typename T>
    T get(const std::string& name) const {
        return mpark::get<T>(get(name));
    }

    /**
     * @brief Get a boolean field value
     *
     * @param name Field name
     */
    template<>
    bool get(const std::string& name) const {
        switch (get_parsed_enum_string(s_bool_parser, name)) {
        case Bool_enum::yes:
            return true;
        case Bool_enum::no:
            return false;
        }
    }

    /**
     * @brief Get a parsed enum field value
     *
     * @tparam T Enum type class
     * @param parser Enum parser
     * @param name Field name
     */
    template<typename T>
    T get_parsed_enum_string(std::function<T(const std::string& name)>& parser,
                             const std::string& name) const {
        const std::string raw{get<String_enum>(name).value};

        return parser(raw);
    }

    /**
     * @brief Get delimiter separated string field value
     *
     * @param name Field name
     * @param delim Delimiter
     */
    std::vector<std::string> get_delim_separated_string(
      const std::string& name, const std::string& delim) const;

    /**
     * @brief Checks if field with given name exists
     *
     * @param name Field name
     */
    bool contains(const std::string& name) const;

private:
    /**
     * @brief Parse a raw input string
     *
     * @param input Raw input string
     * @param variable_resolver Used to resolve variables that are defined in
     * certain elements
     */
    static Container_t parse(const std::string& input,
                             const IVariable_resolver* variable_resolver);

private:
    /**
     * @brief Fields
     */
    Container_t m_fields;

private:
    /**
     * @brief Enum string parser (parses YES/NO values)
     */
    static std::function<Bool_enum(const std::string&)> s_bool_parser;
};

} // namespace m3u8
} // namespace hls

#endif
