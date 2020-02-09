

#include "hls/m3u8/Attribute_list.h"
#include "hls/m3u8/Common.h"

namespace hls {
namespace m3u8 {

std::function<Attribute_list::Bool_enum(const std::string&)>
  Attribute_list::s_bool_parser{[](const std::string& name) {
      return parse_enum_string(
        std::map<std::string, Attribute_list::Bool_enum>{
          {"YES", Bool_enum::yes}, {"NO", Bool_enum::no}},
        name);
  }};


Attribute_list::Attribute_list(const std::string& input)
    : m_fields{parse(input)} {}

Attribute_list::Variant_t Attribute_list::get(const std::string& name) const {
    auto fields_iter{m_fields.find(name)};
    if (fields_iter == m_fields.end()) {
        throw No_such_field_error{"No field named '"s + name + "'"s};
    }

    return fields_iter->second;
}

Attribute_list::Container_t Attribute_list::parse(const std::string& input) {
    Container_t fields;

    std::string raw{input};

    while (!raw.empty()) {
        // Remove whitespace/commas
        if (raw.front() == ',' || raw.front() == ' ') {
            raw = raw.substr(1);
            continue;
        }

        // Find where the attribute value starts
        const size_t value_start_pos{raw.find('=')};
        Expects(value_start_pos != std::string::npos,
                Error{"Could not find value start: '"s + raw + "'"s});

        // Extract name
        const std::string attr_name{raw.substr(0, value_start_pos)};
        raw = raw.substr(value_start_pos + 1);

        // Is it a string ?
        if (raw.front() == '"') {
            // Remove the opening quotes
            raw = raw.substr(1);

            // TODO Does not support strings with escaped quotes
            const size_t closing_quotes_pos{raw.find('"')};
            Expects(closing_quotes_pos != std::string::npos,
                    Error{"Could not find closing quotes: '"s + raw + "'"s});

            const std::string value{raw.substr(0, closing_quotes_pos)};

            // Remove the value along with the closing quotes
            raw = raw.substr(closing_quotes_pos + 1);

            fields.insert({attr_name, value});
            continue;
        }

        // Find the comma which separates us from the next attribute
        const size_t comma_pos{raw.find(',')};


        // If there's no comma, this should be the last one
        const size_t separator_pos{comma_pos == std::string::npos ? raw.size()
                                                                  : comma_pos};

        // Extract raw value
        const std::string value{raw.substr(0, separator_pos)};
        raw = raw.substr(separator_pos);

        // Is it a number?
        if (iwu::String_utils::is_hex_number(value)) {
            // Hex number
            fields.insert({attr_name, strtoll(value.c_str(), nullptr, 16)});
            continue;
        } else if (iwu::String_utils::is_float_number(value)) {
            // Float number
            fields.insert({attr_name, strtof(value.c_str(), nullptr)});
            continue;
        } else if (iwu::String_utils::is_decimal_number(value)) {
            // Integer number
            fields.insert({attr_name, strtoll(value.c_str(), nullptr, 10)});
            continue;
        }

        // Pattern matching resolutions (e.g. "1280x720")
        std::regex resolution_regex{"^([0-9]+)x([0-9]+)$"};
        std::smatch matches;

        // Is it a resolution ?
        if (std::regex_match(value, matches, resolution_regex)) {
            Expects(matches.size() == 3);

            fields.insert(
              {attr_name,
               Resolution{strtoll(matches[1].str().c_str(), nullptr, 10),
                          strtoll(matches[2].str().c_str(), nullptr, 10)}});
            continue;
        }

        // Treat it as a enum string if all else fails
        fields.insert({attr_name, String_enum{value}});
    }

    return fields;
}

bool Attribute_list::contains(const std::string& name) const {
    return m_fields.find(name) != m_fields.cend();
}

std::vector<std::string> Attribute_list::get_delim_separated_string(
  const std::string& name, const std::string& delim) const {
    std::vector<std::string> values;

    iwu::String_utils::split(get<std::string>(name), delim, values);

    return values;
}

} // namespace m3u8
} // namespace hls
