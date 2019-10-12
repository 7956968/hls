#include <hls/m3u8/DefaultParserFactory.h>

#include <iwu/StringUtils.h>
#include <iwu/Log.h>

#include <iostream>
#include <algorithm>
#include <memory>
#include <functional>
#include <map>

#include <iwu/DefaultDownloaderFactory.h>
#include <iwu/VectorDownloaderDestination.h>


int main() {
    //    // Enable logging
    // iwu::Log::setLevel(iwu::Log::eLEVEL_VERBOSE);

    // hls::m3u8::DefaultParserFactory factory;

    // hls::m3u8::IParser* parser = factory.create();

    // iwu::VectorDownloaderDestination buffer;

    // std::unique_ptr<iwu::IDownloader> downloader = std::unique_ptr<iwu::IDownloader>(iwu::DefaultDownloaderFactory().create(
    //     iwu::IDownloader::Configuration()
    //         .setDestination(&buffer)
    // ));

    // iwu::HTTPCode code;

    // const iwu::IDownloader::DownloadStatus drc = downloader->start(
    //     "https://bitdash-a.akamaihd.net/content/sintel/hls/video/250kbit.m3u8", &code);

    // if(drc != iwu::IDownloader::DownloadStatus::OK) {
    //     DLOGE("@3 error ..");
    //     return -1;
    // }


    // std::vector<std::string> lines;
    // iwu::StringUtils::split(buffer.getBufferAsString(), "\n", lines);


    // for(auto& line : lines){
    //     hls::m3u8::AElement* el = parser->parseLine(line);

    //     if(!el){
    //         continue;
    //     }

    //     DLOGD("@# el=%p type=%d", el, el->getType());
    // }


    return 0;
}