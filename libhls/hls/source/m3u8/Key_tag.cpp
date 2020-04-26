#include "hls/m3u8/Key_tag.h"
#include "hls/m3u8/Attribute_list.h"
#include "hls/m3u8/Common.h"

namespace {
constexpr auto k_attr_uri{"URI"};
constexpr auto k_attr_iv{"IV"};
constexpr auto k_attr_method{"METHOD"};
constexpr auto k_attr_key_format{"KEYFORMAT"};
constexpr auto k_attr_key_format_versions{"KEYFORMATVERSIONS"};
} // namespace

namespace hls {
namespace m3u8 {

std::function<Key_tag::Method(const std::string&)> Key_tag::s_method_parser{
  [](const std::string& name) {
      return hls::m3u8::parse_enum_string(
        std::map<std::string, Key_tag::Method>{
          {"NONE", Key_tag::Method::none},
          {"AES-128", Key_tag::Method::aes_128},
          {"SAMPLE-AES", Key_tag::Method::sample_aes}},
        name);
  }};

Key_tag::Key_tag(const std::string& value,
                 const IVariable_resolver* variable_resolver)
    : Tag{Tag::Tag_type::x_key} {
    const Attribute_list al{value, variable_resolver};

    // Mandatory method
    m_method = al.get_parsed_enum_string(s_method_parser, k_attr_method);

    if (m_method == Method::none) {
        return;
    }

    // Mandatory URI
    m_uri = al.get<std::string>(k_attr_uri);

    // TODO Not clear from RFC8216 if this is optional if method != none
    if (al.contains(k_attr_iv)) {
        m_iv = al.get<std::string>(k_attr_iv);
    }

    if (al.contains(k_attr_key_format)) {
        m_key_format = al.get<std::string>(k_attr_key_format);
    }

    if (al.contains(k_attr_key_format_versions)) {
        m_key_format_versions = al.get<std::string>(k_attr_key_format_versions);
    }
}

Key_tag::Method Key_tag::method() const { return m_method; }

const nonstd::optional<std::string> Key_tag::uri() const { return m_uri; }

const nonstd::optional<std::string> Key_tag::iv() const { return m_iv; }

const nonstd::optional<std::string> Key_tag::key_format() const {
    return m_key_format;
}

const nonstd::optional<std::string> Key_tag::key_format_versions() const {
    return m_key_format_versions;
}

} // namespace m3u8
} // namespace hls