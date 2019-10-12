LOCAL_PATH := $(call my-dir)

################################################################################
include $(CLEAR_VARS)

libRoot := $(LOCAL_PATH)/libdash
externalRoot := $(libRoot)/../external
LIBDASH_RAPIDXML_INCLUDE := $(externalRoot)/rapidxml-1.13

ifeq "$(IW_USE_TAG_FOR_COMMON_LIBNAMES)" "YES"
IW_TAG:=_iw
else
IW_TAG:=
endif

includePaths += \
    $(libRoot)/mpd/include \
    $(libRoot)/core/include \
    $(libRoot)/common/include \
    $(LIBDASH_RAPIDXML_INCLUDE) \
    $(CURL_INCLUDE) \
    $(OPENSSL_INCLUDE)

LOCAL_EXPORT_C_INCLUDES += $(includePaths)

LOCAL_CFLAGS += \
    -Werror \
    -Wall \
    -Wno-unused-function \
    -Wno-unused-variable

LOCAL_CPPFLAGS += -frtti -fexceptions

LOCAL_SRC_FILES += \
    libdash/mpd/source/ANode.cpp \
    libdash/mpd/source/AdaptationSet.cpp \
    libdash/mpd/source/BaseUrl.cpp \
    libdash/mpd/source/Descriptor.cpp \
    libdash/mpd/source/Event.cpp \
    libdash/mpd/source/EventStream.cpp \
    libdash/mpd/source/Factory.cpp \
    libdash/mpd/source/Manager.cpp \
    libdash/mpd/source/Metrics.cpp \
    libdash/mpd/source/Mpd.cpp \
    libdash/mpd/source/MultipleSegmentBase.cpp \
    libdash/mpd/source/Period.cpp \
    libdash/mpd/source/ProgramInformation.cpp \
    libdash/mpd/source/Range.cpp \
    libdash/mpd/source/Representation.cpp \
    libdash/mpd/source/RepresentationBase.cpp \
    libdash/mpd/source/SegmentBase.cpp \
    libdash/mpd/source/SegmentList.cpp \
    libdash/mpd/source/SegmentTemplate.cpp \
    libdash/mpd/source/SegmentTimeline.cpp \
    libdash/mpd/source/SegmentUrl.cpp \
    libdash/mpd/source/Subset.cpp \
    libdash/mpd/source/Url.cpp \
    libdash/core/source/ABaseAbrAlgorithm.cpp  \
    libdash/core/source/AdaptationStream.cpp  \
    libdash/core/source/AdaptationStreamDownloader.cpp  \
    libdash/core/source/DefaultAbrAlgorithm.cpp  \
    libdash/core/source/Factory.cpp \
    libdash/core/source/RandomAbrAlgorithm.cpp  \
    libdash/core/source/RepresentationStreamFactory.cpp  \
    libdash/core/source/SegmentTemplateStream.cpp \
    libdash/core/source/SingleFileStream.cpp  \
    libdash/core/source/SingleSegmentStream.cpp  \
    libdash/core/source/StreamManager.cpp \
    libdash/core/source/StreamManagerDebugger.cpp  \
    libdash/core/source/SyncBarrier.cpp \
    libdash/core/source/Segment.cpp \
    libdash/core/source/SegmentListStream.cpp

LOCAL_C_INCLUDES += $(includePaths)

LOCAL_STATIC_LIBRARIES += \
    libcurl_iw libssl_iw libcrypto_iw

ifeq "$(IW_USE_INTERNAL_SSL_CURL)" "YES"
LOCAL_STATIC_LIBRARIES += \
        libcurl_iw \
        libssl_iw \
        libcrypto_iw
else
LOCAL_SHARED_LIBRARIES += \
        libcurl
endif

LOCAL_SHARED_LIBRARIES += \
    libiwu

ifeq "$(IW_USE_VENDOR_PARTITION)" "YES"
LOCAL_PROPRIETARY_MODULE := true
endif

LOCAL_LDLIBS += \
    -lz \
    -llog

LOCAL_MODULE := libdash$(IW_TAG)

include $(BUILD_SHARED_LIBRARY)
################################################################################
BUILD_TEST_SAMPLE:="false"
ifeq "$(BUILD_TEST_SAMPLE)" "true"
include $(CLEAR_VARS)

libRoot := $(LOCAL_PATH)/libdash


LOCAL_SRC_FILES += \
    $(wildcard $(libRoot)/sample/source/samples/*.cpp) \
    $(wildcard $(libRoot)/sample/source/framework/*.cpp)

LOCAL_C_INCLUDES += \
    $(libRoot)/sample/source/framework

LOCAL_SHARED_LIBRARIES += \
    libdash

LOCAL_LDLIBS += \
    -lz \
    -llog

LOCAL_MODULE := dash_sample

include $(BUILD_EXECUTABLE)
endif
