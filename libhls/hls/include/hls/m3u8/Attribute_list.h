#ifndef HLS_M3U8ATTRIBUTELIST_H_
#define HLS_M3U8ATTRIBUTELIST_H_

#include "hls/Common.h"
#include "hls/m3u8/IVariable_resolver.h"

#include <regex>
#include <unordered_map>
#include <variant.hpp>

namespace hls {
namespace m3u8 {

class Attribute_list {
public:
    enum class Bool_enum { yes, no };

    using Resolution = std::pair<long, long>;

    struct String_enum {
        std::string value;

        explicit String_enum(const std::string& value) : value{value} {}
    };

    using Variant_t =
      mpark::variant<long long, float, std::string, Resolution, String_enum>;

    using Container_t = std::unordered_map<std::string, Variant_t>;

    static constexpr gsl::index k_variant_number_integer_i{0};
    static constexpr gsl::index k_variant_number_float_i{1};
    static constexpr gsl::index k_variant_string_i{2};
    static constexpr gsl::index k_variant_resolution_i{3};
    static constexpr gsl::index k_variant_enum_string_i{4};

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

    Variant_t get(const std::string& name) const;

    template<typename T>
    T get(const std::string& name) const {
        return mpark::get<T>(get(name));
    }

    template<>
    bool get(const std::string& name) const {
        switch (get_parsed_enum_string(s_bool_parser, name)) {
        case Bool_enum::yes:
            return true;
        case Bool_enum::no:
            return false;
        }
    }

    template<typename T>
    T get_parsed_enum_string(std::function<T(const std::string& name)>& parser,
                             const std::string& name) const {
        const std::string raw{get<String_enum>(name).value};

        return parser(raw);
    }

    std::vector<std::string> get_delim_separated_string(
      const std::string& name, const std::string& delim) const;

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
    Container_t m_fields;

    static std::function<Bool_enum(const std::string&)> s_bool_parser;
};

} // namespace m3u8
} // namespace hls

#endif
