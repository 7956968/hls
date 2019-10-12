#include "ATest.h"

#include <iwu/Log.h>

#include <unistd.h>
#include <linux/limits.h>
#include <fstream>

// TODO Assumes we're calling from library root
// should probably make this command line argument
#define ASSETS_DIR "/libiwu/test/assets"

std::string ATest::getAssetPath(const std::string& name) const {
    char currentDir[PATH_MAX];
    getcwd(currentDir, PATH_MAX);

    return std::string(currentDir) + ASSETS_DIR "/" + name;
}

bool ATest::loadFile(const std::string& name,
        std::vector<uint8_t>& buffer) const {
    const std::string fullPath = getAssetPath(name);

    std::ifstream file(fullPath.c_str(), std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer.resize(size);

    if(file.good()) {
        file.read(reinterpret_cast<char*>(buffer.data()), size);
        return true;
    } else {
        return false;
    }
}
