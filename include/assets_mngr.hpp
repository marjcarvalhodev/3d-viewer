#ifndef ASSETS_MNGR_HPP
#define ASSETS_MNGR_HPP

#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

class AssetsManager {
private:
    std::string basePath;

public:

    AssetsManager(const std::string& basePath = "assets");
    ~AssetsManager();

    std::string readFile(const std::string& fileName) const;
};

#endif