
#include "hls/m3u8/Stream_inf_tag.h"

#include "iwu/test/Test_common.h"

class TestStreamInfTag : public ::testing::Test {};

TEST_F(TestStreamInfTag, BandwidthMissing) {
    ASSERT_THROW(hls::m3u8::Stream_inf_tag(""s, nullptr), hls::Error);
}

TEST_F(TestStreamInfTag, Bandwidth) {
    const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=12345", nullptr};
    ASSERT_EQ(12345, tag.bandwidth());
}

TEST_F(TestStreamInfTag, AverageBandwidth) {
    {
        const hls::m3u8::Stream_inf_tag tag{
          "BANDWIDTH=0,AVERAGE-BANDWIDTH=12345", nullptr};

        ASSERT_EQ(12345, tag.average_bandwidth().value());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0", nullptr};
        ASSERT_FALSE(tag.average_bandwidth());
    }
}

TEST_F(TestStreamInfTag, Codecs) {
    {
        const hls::m3u8::Stream_inf_tag tag{
          "BANDWIDTH=0,CODECS=\"mp4a.40.2,avc1.4d401e\"", nullptr};

        std::vector<std::string> codecs{tag.codecs().value()};

        ASSERT_EQ(codecs.size(), 2);

        ASSERT_EQ(codecs[0], "mp4a.40.2");
        ASSERT_EQ(codecs[1], "avc1.4d401e");
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0", nullptr};

        ASSERT_FALSE(tag.codecs());
    }
}

TEST_F(TestStreamInfTag, Resolution) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0", nullptr};
        ASSERT_FALSE(tag.resolution());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,RESOLUTION=1280x720",
                                            nullptr};
        ASSERT_EQ(std::make_pair(1280L, 720L), tag.resolution().value());
    }
}

TEST_F(TestStreamInfTag, FrameRate) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0", nullptr};
        ASSERT_FALSE(tag.frame_rate());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,FRAME-RATE=3.14",
                                            nullptr};
        ASSERT_FLOAT_EQ(3.14, tag.frame_rate().value());
    }
}

TEST_F(TestStreamInfTag, HdcpLevel) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,HDCP-LEVEL=TYPE-0",
                                            nullptr};
        ASSERT_EQ(hls::m3u8::Stream_inf_tag::Hdcp_level::type_0,
                  tag.hdcp_level().value());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,HDCP-LEVEL=TYPE-1",
                                            nullptr};
        ASSERT_EQ(hls::m3u8::Stream_inf_tag::Hdcp_level::type_1,
                  tag.hdcp_level().value());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,HDCP-LEVEL=TYPE-NONE",
                                            nullptr};
        ASSERT_EQ(hls::m3u8::Stream_inf_tag::Hdcp_level::none,
                  tag.hdcp_level().value());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0", nullptr};
        ASSERT_FALSE(tag.hdcp_level());
    }

    ASSERT_THROW(
      hls::m3u8::Stream_inf_tag("BANDWIDTH=0,HDCP-LEVEL=asdf", nullptr),
      iwu::Error);
}

TEST_F(TestStreamInfTag, AllowedCpc) {
    const hls::m3u8::Stream_inf_tag tag{
      "BANDWIDTH=0,ALLOWED-CPC=\"com.example.drm1:SMART-TV/"
      "PC,com.example.drm2: HW\"",
      nullptr};

    std::vector<std::string> codecs{tag.allowed_cpc().value()};

    ASSERT_EQ(codecs.size(), 2);

    ASSERT_EQ(codecs[0], "com.example.drm1:SMART-TV/PC");
    ASSERT_EQ(codecs[1], "com.example.drm2: HW");
}

TEST_F(TestStreamInfTag, VideoRange) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,VIDEO-RANGE=SDR",
                                            nullptr};
        ASSERT_EQ(hls::m3u8::Stream_inf_tag::Video_range::sdr,
                  tag.video_range());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,VIDEO-RANGE=PQ",
                                            nullptr};
        ASSERT_EQ(hls::m3u8::Stream_inf_tag::Video_range::pq,
                  tag.video_range());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0", nullptr};
        ASSERT_FALSE(tag.video_range());
    }


    ASSERT_THROW(
      hls::m3u8::Stream_inf_tag("BANDWIDTH=0,VIDEO-RANGE=asdf", nullptr),
      hls::Error);
}

TEST_F(TestStreamInfTag, Video) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,VIDEO=\"video\"",
                                            nullptr};
        ASSERT_EQ("video", tag.video());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0", nullptr};
        ASSERT_FALSE(tag.video());
    }
}

TEST_F(TestStreamInfTag, Audio) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,AUDIO=\"audio\"",
                                            nullptr};
        ASSERT_EQ("audio", tag.audio());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0", nullptr};
        ASSERT_FALSE(tag.audio());
    }
}


TEST_F(TestStreamInfTag, Subtitles) {
    {
        const hls::m3u8::Stream_inf_tag tag{
          "BANDWIDTH=0,SUBTITLES=\"subtitles\"", nullptr};
        ASSERT_EQ("subtitles", tag.subtitles());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0", nullptr};
        ASSERT_FALSE(tag.subtitles());
    }
}

TEST_F(TestStreamInfTag, ClosedCaptions) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,CLOSED-CAPTIONS=NONE",
                                            nullptr};
        ASSERT_EQ(std::make_pair(false, ""s), tag.closed_captions().value());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{
          "BANDWIDTH=0,CLOSED-CAPTIONS=\"closed_captions\"", nullptr};
        ASSERT_EQ(std::make_pair(true, "closed_captions"s),
                  tag.closed_captions().value());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0", nullptr};
        ASSERT_FALSE(tag.closed_captions());
    }
}