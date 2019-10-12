#ifndef HLS_M3U8_PARSER_H_
#define HLS_M3U8_PARSER_H_

#include <hls/m3u8/AttributeList.h>
#include <hls/m3u8/AElement.h>
#include <hls/m3u8/Uri.h>
#include <hls/m3u8/IntegerTag.h>
#include <hls/m3u8/Comment.h>

#include <string>

namespace hls {

namespace m3u8 {



static const std::string kTAG_PREFIX = "#EXT";


class Parser : public IParser {

public:
    const AElement* parseLine(const std::string& inLine) override {
        std::string line = inLine;

        line = iwu::StringUtils::trim(line);
        if(line.empty()) {
            return nullptr;
        }

        if(iwu::StringUtils::startsWith(line, kTAG_PREFIX)) {
            // Tag
            Tag* tag = parseTag(line);
            if(!tag) {
                DILOGE("Error parsing tag");
                return nullptr;
            }

            return tag;
        }
        else if(*line.begin() == '#') {
            // Comment
            return new Comment(line.substr(1));
        }
        else {
            // Uri
            return new Uri(line);
        }
    }


    typedef std::function<Tag*(const std::string& body)> TagParserFnc;


    TagParserFnc createIntegerTagParser(Tag::TagType type) {
        return TagParserFnc([type](const std::string& value) -> Tag* {
            std::unique_ptr<IntegerTag> tag(new IntegerTag(type));
            if(!tag->parse(value)) {
                return nullptr;
            }

            return tag.release();
        });
    }
    Tag* parseTag(const std::string& lineIn) {
        std::string line = lineIn;

        // Extract tag name
        std::size_t tagNameEnd = line.find(':');
        tagNameEnd == std::string::npos ?
            line.size() -1 : tagNameEnd;

        const std::string tagName = line.substr(kTAG_PREFIX.size(), tagNameEnd-kTAG_PREFIX.size());

        line = line.substr(tagNameEnd+1);

        auto iter = mRegisteredTags.find(tagName);
        if(iter == mRegisteredTags.end()){
            DILOGE("Tag not supported '%s'", tagName.c_str());
            return nullptr;
        }

        if(iter->second.parser) {
            auto tag = iter->second.parser(line);
            if(!tag){
                DILOGE("tag parse failed: %s", line.c_str());
                return nullptr;
            }

            return tag;
        }

        return new Tag(iter->second.type);
    }

    template<typename T>
    Parser& registerParsedTagType(const std::string& name, Tag::TagType type) {
        return registerTagType(name, type, std::bind(&T::parse, std::placeholders::_1));
    }

    Parser& registerTagType(const std::string& name, Tag::TagType type, const TagParserFnc& parser=nullptr) {
        mRegisteredTags.insert(
            std::make_pair(name, TagEntry(name, type, parser))
        );
        return *this;
    }

    Parser& registerIntegerTagType(const std::string& name, Tag::TagType type) {
        return registerTagType(name, type, createIntegerTagParser(type));
    }
private:
    struct TagEntry {
        std::string name;
        Tag::TagType type;
        TagParserFnc parser;

        TagEntry(const std::string& name, Tag::TagType type, const TagParserFnc& parser=nullptr) : name(name),
            type(type), parser(parser) {
        }
    };

    std::map<const std::string, TagEntry> mRegisteredTags;
};


} // m3u8

} // hls

#endif