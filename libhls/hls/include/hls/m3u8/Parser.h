#ifndef HLS_M3U8_PARSER_H_
#define HLS_M3U8_PARSER_H_

#include "hls/m3u8/IParser.h"
#include "hls/m3u8/Tag.h"

#include <functional>
#include <map>

namespace hls {
namespace m3u8 {

class Parser : public IParser {
public:
    Parser();

public:
    std::unique_ptr<const AElement> parse_line(
      const std::string& in_line) override;

private:
    using Tag_parser_fnc =
      std::function<std::unique_ptr<Tag>(const std::string& body)>;

    struct Tag_entry {
        std::string name;
        Tag::Tag_type type;
        Tag_parser_fnc parser;

        Tag_entry(const std::string& name, Tag::Tag_type type,
                  const Tag_parser_fnc& parser)
            : name{name}, type{type}, parser{parser} {}
    };

private:
    std::unique_ptr<Tag> parse_tag(const std::string& line);

    template<typename T>
    Parser& register_specialized_tag_type(const std::string& name,
                                          Tag::Tag_type type) {
        return register_tag_type(name, type, [](const std::string& value) {
            return std::make_unique<T>(value);
        });
    }

    Parser& register_tag_type(const std::string& name, Tag::Tag_type type,
                              const Tag_parser_fnc& parser = nullptr);

    Parser& register_integer_tag_type(const std::string& name,
                                      Tag::Tag_type type);

private:
    std::map<const std::string, Tag_entry> m_registered_tags;
};


// };

} // namespace m3u8
} // namespace hls

#endif