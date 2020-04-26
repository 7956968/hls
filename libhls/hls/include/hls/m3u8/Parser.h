#ifndef HLS_M3U8_PARSER_H_
#define HLS_M3U8_PARSER_H_

#include "hls/m3u8/Common.h"
#include "hls/m3u8/Enum_tag.h"
#include "hls/m3u8/IParser.h"
#include "hls/m3u8/IVariable_repository.h"
#include "hls/m3u8/IVariable_resolver.h"
#include "hls/m3u8/Tag.h"

#include <functional>
#include <map>

namespace hls {
namespace m3u8 {

/**
 * @brief M3U8 playlist parser
 */
class Parser : public IParser {
public:
    /**
     * @brief Constructor
     */
    Parser();

    /**
     * @brief IParser implementation
     */
public:
    std::unique_ptr<const AElement> parse_line(
      const std::string& in_line) override;

    void set_variable_repository(IVariable_repository* repository) override;

    void set_variable_resolver(
      const IVariable_resolver* variable_resolver) override;

private:
    /**
     * @brief Function which given a raw string, instantiates a tag
     */
    using Tag_parser_fnc =
      std::function<std::unique_ptr<Tag>(const std::string& body)>;

    /**
     * @brief Parsed tag entry
     */
    struct Tag_entry {
        /**
         * @brief Tag name
         */
        std::string name;

        /**
         * @brief Tag type
         */
        Tag::Tag_type type;

        /**
         * @brief Tag parser function
         */
        Tag_parser_fnc parser;

        Tag_entry(const std::string& name, Tag::Tag_type type,
                  const Tag_parser_fnc& parser)
            : name{name}, type{type}, parser{parser} {}
    };

private:
    /**
     * @brief Parse a tag, given a raw line
     *
     * @param line Raw line
     * @return Parsed tag
     */
    std::unique_ptr<Tag> parse_tag(const std::string& line);

    /**
     * @brief Register a specialized tag type (i.e. which has its own class)
     *
     * @tparam T Tag class
     * @param name Tag name
     * @param type Tag type
     */
    template<typename T>
    Parser& register_specialized_tag_type(const std::string& name,
                                          Tag::Tag_type type) {
        return register_tag_type(name, type, [this](const std::string& value) {
            return std::make_unique<T>(value, m_variable_resolver);
        });
    }

    /**
     * @brief Register enum tag (i.e. specialization of the hls::m3u8::Enum_tag)
     *
     * @tparam T Enum class
     * @param name Tag name
     * @param type Tag type
     * @param parser_fnc Enum parser function
     */
    template<typename T>
    Parser& register_enum_tag(const std::string& name, Tag::Tag_type type,
                              Enum_parser_fnc<T> parser_fnc) {
        return register_tag_type(
          name, type, [type, parser_fnc](const std::string& value) {
              return std::make_unique<Enum_tag<T>>(value, type, parser_fnc);
          });
    }

    /**
     * @brief Register a generic tag type
     *
     * @param name Tag name
     * @param type Tag type
     * @param parser Tag parser function
     */
    Parser& register_tag_type(const std::string& name, Tag::Tag_type type,
                              const Tag_parser_fnc& parser = nullptr);

    /**
     * @brief Register an integer tag type
     *
     * @param name Tag name
     * @param type Tag type
     */
    Parser& register_integer_tag_type(const std::string& name,
                                      Tag::Tag_type type);

    /**
     * @brief Register a generic tag type
     *
     * @param name Tag name
     * @param entry Tag entry
     */
    Parser& register_tag_type(const std::string& name, Tag_entry&& entry);

private:
    /**
     * @brief Supported tags
     */
    std::map<const std::string, Tag_entry> m_registered_tags;

    /**
     * @brief Variable resolver
     */
    const IVariable_resolver* m_variable_resolver{nullptr};

    /**
     * @brief Variable repository
     */
    IVariable_repository* m_variable_repository{nullptr};
};


} // namespace m3u8
} // namespace hls

#endif