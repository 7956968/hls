#include "hls/m3u8/Define_tag.h"
#include "hls/m3u8/Attribute_list.h"

namespace {
constexpr auto k_attr_name{"NAME"};
constexpr auto k_attr_value{"VALUE"};
constexpr auto k_attr_import{"IMPORT"};
} // namespace

namespace hls {
namespace m3u8 {

Define_tag::Define_tag(const std::string& value,
                       const IVariable_resolver* variable_resolver)
    : Tag{Tag_type::x_define} {
    const Attribute_list al{value, variable_resolver};

    if (al.contains(k_attr_name)) {
        m_name = al.get<std::string>(k_attr_name);

        // Value is required if name is defined
        m_value = al.get<std::string>(k_attr_value);

        return;
    }

    // Import is required if name+value is not defined
    m_import_name = al.get<std::string>(k_attr_import);
}

nonstd::optional<std::string> Define_tag::name() const { return m_name; }

nonstd::optional<std::string> Define_tag::value() const { return m_value; }

nonstd::optional<std::string> Define_tag::import_name() const {
    return m_import_name;
}

} // namespace m3u8
} // namespace hls
