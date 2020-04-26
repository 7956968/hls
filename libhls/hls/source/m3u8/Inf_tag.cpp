#include "hls/m3u8/Inf_tag.h"

#include "iwu/String_utils.h"

namespace hls {
namespace m3u8 {

Inf_tag::Inf_tag(const std::string& value, const IVariable_resolver*)
    : Tag{Tag_type::inf} {
    std::vector<std::string> tokens;
    iwu::String_utils::split(value, ",", tokens);

    Expects(tokens.size() == 1 || tokens.size() == 2,
            Parse_error{"Invalid inf tag body '"s + value + "'"s});

    // Parse the duration
    const std::string& duration_str{tokens[0]};
    if (iwu::String_utils::is_float_number(duration_str)) {
        m_duration = strtod(duration_str.c_str(), nullptr);
    } else if (iwu::String_utils::is_decimal_number(duration_str)) {
        m_duration = strtol(duration_str.c_str(), nullptr, 10);
    } else {
        throw Parse_error{"Unrecognized inf tag duration: '"s + duration_str
                          + "'"s};
    }

    // Optional title
    if (tokens.size() == 2) {
        m_title = tokens[1];
    }
}

double Inf_tag::duration() const { return m_duration; }

nonstd::optional<std::string> Inf_tag::title() const { return m_title; }


} // namespace m3u8
} // namespace hls