
#include "Common.h"

#include "hls/m3u8/Stream_inf_tag.h"

class TestStreamInfTag : public ::testing::Test {};

TEST_F(TestStreamInfTag, BandwidthMissing) {
    ASSERT_THROW(const hls::m3u8::Stream_inf_tag tag{""}, hls::Error);
}

TEST_F(TestStreamInfTag, Bandwidth) {
    const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=12345"};
    ASSERT_EQ(12345, tag.bandwidth());
}

TEST_F(TestStreamInfTag, AverageBandwidth) {
    {
        const hls::m3u8::Stream_inf_tag tag{
          "BANDWIDTH=0,AVERAGE-BANDWIDTH=12345"};
        ASSERT_EQ(12345, tag.average_bandwidth());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0"};
        ASSERT_EQ(-1, tag.average_bandwidth());
    }
}

TEST_F(TestStreamInfTag, Codecs) {
    const hls::m3u8::Stream_inf_tag tag{
      "BANDWIDTH=0,CODECS=\"mp4a.40.2,avc1.4d401e\""};

    std::vector<std::string> codecs{tag.codecs()};

    ASSERT_EQ(codecs.size(), 2);

    ASSERT_EQ(codecs[0], "mp4a.40.2");
    ASSERT_EQ(codecs[1], "avc1.4d401e");
}

TEST_F(TestStreamInfTag, Resolution) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0"};
        ASSERT_EQ(std::make_pair(-1L, -1L), tag.resolution());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,RESOLUTION=1280x720"};
        ASSERT_EQ(std::make_pair(1280L, 720L), tag.resolution());
    }
}

TEST_F(TestStreamInfTag, FrameRate) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0"};
        ASSERT_FLOAT_EQ(-1.0, tag.frame_rate());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,FRAME-RATE=3.14"};
        ASSERT_FLOAT_EQ(3.14, tag.frame_rate());
    }
}

TEST_F(TestStreamInfTag, HdcpLevel) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,HDCP-LEVEL=TYPE-0"};
        ASSERT_EQ(hls::m3u8::Stream_inf_tag::Hdcp_level::type_0,
                  tag.hdcp_level());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,HDCP-LEVEL=TYPE-1"};
        ASSERT_EQ(hls::m3u8::Stream_inf_tag::Hdcp_level::type_1,
                  tag.hdcp_level());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,HDCP-LEVEL=TYPE-NONE"};
        ASSERT_EQ(hls::m3u8::Stream_inf_tag::Hdcp_level::none,
                  tag.hdcp_level());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0"};
        ASSERT_EQ(hls::m3u8::Stream_inf_tag::Hdcp_level::not_set,
                  tag.hdcp_level());
    }

    ASSERT_THROW(hls::m3u8::Stream_inf_tag{"BANDWIDTH=0,HDCP-LEVEL=asdf"},
                 iwu::Error);
}

TEST_F(TestStreamInfTag, AllowedCpc) {
    const hls::m3u8::Stream_inf_tag tag{
      "BANDWIDTH=0,ALLOWED-CPC=\"com.example.drm1:SMART-TV/"
      "PC,com.example.drm2: HW\""};

    std::vector<std::string> codecs{tag.allowed_cpc()};

    ASSERT_EQ(codecs.size(), 2);

    ASSERT_EQ(codecs[0], "com.example.drm1:SMART-TV/PC");
    ASSERT_EQ(codecs[1], "com.example.drm2: HW");
}

TEST_F(TestStreamInfTag, VideoRange) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,VIDEO-RANGE=SDR"};
        ASSERT_EQ(hls::m3u8::Stream_inf_tag::Video_range::sdr,
                  tag.video_range());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,VIDEO-RANGE=PQ"};
        ASSERT_EQ(hls::m3u8::Stream_inf_tag::Video_range::pq,
                  tag.video_range());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0"};
        ASSERT_EQ(hls::m3u8::Stream_inf_tag::Video_range::not_set,
                  tag.video_range());
    }


    ASSERT_THROW(hls::m3u8::Stream_inf_tag{"BANDWIDTH=0,VIDEO-RANGE=asdf"},
                 hls::Error);
}

TEST_F(TestStreamInfTag, Video) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,VIDEO=\"video\""};
        ASSERT_EQ("video", tag.video());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0"};
        ASSERT_TRUE(tag.video().empty());
    }
}

TEST_F(TestStreamInfTag, Audio) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,AUDIO=\"audio\""};
        ASSERT_EQ("audio", tag.audio());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0"};
        ASSERT_TRUE(tag.audio().empty());
    }
}


TEST_F(TestStreamInfTag, Subtitles) {
    {
        const hls::m3u8::Stream_inf_tag tag{
          "BANDWIDTH=0,SUBTITLES=\"subtitles\""};
        ASSERT_EQ("subtitles", tag.subtitles());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0"};
        ASSERT_TRUE(tag.subtitles().empty());
    }
}

TEST_F(TestStreamInfTag, ClosedCaptions) {
    {
        const hls::m3u8::Stream_inf_tag tag{"BANDWIDTH=0,CLOSED-CAPTIONS=NONE"};
        ASSERT_EQ(std::make_pair(false, ""s), tag.closed_captions());
    }

    {
        const hls::m3u8::Stream_inf_tag tag{
          "BANDWIDTH=0,CLOSED-CAPTIONS=\"closed_captions\""};
        ASSERT_EQ(std::make_pair(true, "closed_captions"s),
                  tag.closed_captions());
    }
}