#ifndef HLS_M3U8ATTRIBUTELIST_H_
#define HLS_M3U8ATTRIBUTELIST_H_

#include "hls/Common.h"

#include <regex>
#include <unordered_map>
#include <variant.hpp>

namespace hls {
namespace m3u8 {

class Attribute_list {
public:
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
    explicit Attribute_list(const std::string& input);

    Variant_t get(const std::string& name) const;

    template<typename T>
    T get(const std::string& name) const {
        return mpark::get<T>(get(name));
    }

    template<>
    bool get(const std::string& name) const {
        const auto str{get<String_enum>(name)};

        if (str.value == "YES") {
            return true;
        } else if (str.value == "NO") {
            return false;
        }

        throw Error{"Invalid bool value '"s + str.value + "'"s};
    }

    bool contains(const std::string& name) const;

private:
    /**
     * @brief Parse a raw input string
     *
     * @param input
     */
    static Container_t parse(const std::string& input);

private:
    Container_t m_fields;
};

} // namespace m3u8
} // namespace hls

#endif
