#ifndef HLS_M3U8_MEDIATAG_H_
#define HLS_M3U8_MEDIATAG_H_

#include <hls/m3u8/Tag.h>
#include <hls/m3u8/AttributeList.h>

namespace hls {

namespace m3u8 {

class MediaTag : public Tag {
public:

    enum class Type {
        AUDIO,
        VIDEO,
        SUBTITLES,
        CLOSED_CAPTIONS,
        UNKOWN
    };

    MediaTag() : Tag(Tag::TagType::X_MEDIA), mType(Type::UNKOWN), mDefault(false), mAutoSelect(false),
            mForced(false) {
    }


    static Tag* parse(const std::string& value) {
        MediaTag* tag = new MediaTag;

        AttributeList al;
        al.fromString(value);

        std::string type;
        if(al.getEnumString("TYPE", &type)) {
            if(type == "AUDIO") {
                tag->mType = Type::AUDIO;
            }
            else if(type == "VIDEO") {
                tag->mType = Type::VIDEO;
            }
            else if(type == "SUBTITLES") {
                tag->mType = Type::SUBTITLES;
            }
            else if(type == "CLOSED_CAPTIONS") {
                tag->mType = Type::CLOSED_CAPTIONS;
            }
            else {
                DLOGE("Unsupported type: '%s'", type.c_str());
                delete tag;
                return nullptr;
            }
        }

        al.getString("URI", &tag->mUri);
        al.getString("GROUP-ID", &tag->mGroupId);
        al.getString("LANGUAGE", &tag->mLanguage);
        al.getString("ASSOC-LANGUAGE", &tag->mAssocLanguage);
        al.getString("NAME", &tag->mName);

        al.getBoolEnum("DEFAULT", &tag->mDefault);
        al.getBoolEnum("AUTOSELECT", &tag->mAutoSelect);
        al.getBoolEnum("FORCED", &tag->mForced);

        al.getString("INSTREAM-ID", &tag->mInStreamId);
        al.getString("CHARACTERISTICS", &tag->mCharacteristics);
        al.getString("CHANNELS", &tag->mChannels);

        return tag;
    }


    Type mType;
    std::string mUri;
    std::string mGroupId;
    std::string mLanguage;
    std::string mAssocLanguage;
    std::string mName;
    bool mDefault;
    bool mAutoSelect;
    bool mForced;
    std::string mInStreamId;
    std::string mCharacteristics;
    std::string mChannels;
};

} // m3u8

} // hls

#endif
