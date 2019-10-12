#ifndef HLS_M3U8_ATTRIBUTELIST_H_
#define HLS_M3U8_ATTRIBUTELIST_H_

#include <hls/Common.h>

#include <iwu/StringUtils.h>
#include <iwu/Log.h>

#include <stdint.h>

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>

namespace hls {

namespace m3u8 {

class AttributeList {
public:
    enum class AttributeType {
        INTEGER,
        FLOAT,
        STRING,
        RESOLUTION,
        ENUM_STRING,
        UNKOWN,
    };

    struct Attribute {
        AttributeType type;

        int64_t integerValue;
        float floatValue;
        std::string stringValue;
        std::string enumStringValue;
        std::pair<int32_t, int32_t> resolutionValue;
    };

    std::map<const std::string, const Attribute> mAttributes;


    const std::map<const std::string, const Attribute> getAttributes() const {
        return mAttributes;
    }

    Status fromString(const std::string& rawIn){
        mAttributes.clear();

        return parseRawAttrs(rawIn, &mAttributes);
    }

    bool getInteger(const std::string& name, int64_t* value) const {
        auto iter = mAttributes.find(name);
        if(iter == mAttributes.end() || iter->second.type != AttributeType::INTEGER) {
            return false;
        }

        *value = iter->second.integerValue;

        return true;
    }

    bool getEnumString(const std::string& name, std::string* value) const {
        auto iter = mAttributes.find(name);
        if(iter == mAttributes.end() || iter->second.type != AttributeType::ENUM_STRING) {
            return false;
        }

        *value = iter->second.enumStringValue;

        return true;
    }

    bool getResolution(const std::string& name, std::pair<int32_t, int32_t>* value) const {
        auto iter = mAttributes.find(name);
        if(iter == mAttributes.end() || iter->second.type != AttributeType::RESOLUTION) {
            return false;
        }

        *value = iter->second.resolutionValue;

        return true;
    }

    bool getBoolEnum(const std::string& name, bool* value) {
        std::string strValue;
        if(getEnumString(name, &strValue)){

            DILOGD("@# enam '%s'", strValue.c_str());

            if(strValue == "YES") {
                *value = true;
                return true;
            }
            else if(strValue == "NO") {
                *value = false;
                return true;
            }
        }

        return false;
    }

    bool getString(const std::string& name, std::string* value) const {
        auto iter = mAttributes.find(name);
        if(iter == mAttributes.end() || iter->second.type != AttributeType::STRING) {
            return false;
        }

        *value = iter->second.stringValue;

        return true;
    }

    bool getFloat(const std::string& name, float* value) const {
        auto iter = mAttributes.find(name);
        if(iter == mAttributes.end() || iter->second.type != AttributeType::FLOAT) {
            return false;
        }

        *value = iter->second.floatValue;

        return true;
    }

    Status parseRawAttrs(const std::string& rawIn,  std::map<const std::string, const Attribute>* attributes) {
        std::vector<std::string> tokens;

        std::string raw = rawIn;

        while(!raw.empty()){
            std::size_t valueStartPos = raw.find('=');

            const std::string attributeName = raw.substr(0, valueStartPos);

            raw = raw.substr(valueStartPos+1);

            if(*raw.begin() == '"') {
                raw = raw.substr(1);
                // TODO Support escaped quotes
                std::size_t quotesPos = raw.find('"');

                const std::string value = raw.substr(0, quotesPos);

                // TODO find ',' properly
                raw = raw.substr(quotesPos+1);

                if(!raw.empty()){
                    std::size_t commaPos = raw.find(',');
                    if(commaPos != std::string::npos) {
                        raw = raw.substr(commaPos+1);
                    }
                }
                Attribute attribute;
                attribute.type = AttributeType::STRING;
                attribute.stringValue = value;

                attributes->insert(std::make_pair(attributeName,
                    attribute));


            }
            else {
                std::size_t commaPos = raw.find(',');

                commaPos = commaPos == std::string::npos ? raw.size() - 1 : commaPos;

                const std::string value = raw.substr(0, commaPos);

                raw = raw.substr(commaPos + 1);

                Attribute attribute;

                if(iwu::StringUtils::isHexNumber(value)){
                    // Decimal
                    attribute.type = AttributeType::INTEGER;
                    attribute.integerValue = strtoll(value.c_str(), nullptr, 16);
                } else if(iwu::StringUtils::isFloatNumber(value)) {
                    // Float
                    attribute.type = AttributeType::FLOAT;
                    attribute.floatValue = strtof(value.c_str(), nullptr);
                } else if(iwu::StringUtils::isDecimalNumber(value)) {
                    // Decimal
                    attribute.type = AttributeType::INTEGER;
                    attribute.integerValue = strtoll(value.c_str(), nullptr, 10);
                }
                else {

                    if(std::count(value.begin(), value.end(), 'x') == 1) {
                        // Resolution
                        std::vector<std::string> tokens;
                        iwu::StringUtils::split(value, "x", tokens);

                        if(tokens.size() != 2) {
                            DLOGE("Sanity check failed");
                            return Status::ERROR;
                        }

                        if(iwu::StringUtils::isDecimalNumber(tokens[0]),
                                iwu::StringUtils::isDecimalNumber(tokens[1])){
                             attribute.type = AttributeType::RESOLUTION;
                             attribute.resolutionValue = std::make_pair(atoi(tokens[0].c_str()), atoi(tokens[1].c_str()));
                        }
                    }

                    if(attribute.type != AttributeType::RESOLUTION) {
                        // Enum string
                        attribute.type = AttributeType::ENUM_STRING;
                        attribute.enumStringValue = value;
                    }
                }

                attributes->insert(std::make_pair(attributeName,
                    attribute));
            }

        }

        return Status::OK;
    }
};

} // m3u8

} // hls

#endif