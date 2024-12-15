#include "assets_mngr.hpp"

namespace fs = std::filesystem;

AssetsManager::AssetsManager(const std::string &basePath)
{
    if (!fs::exists(basePath))
    {
        throw std::runtime_error("Assets directory not found: " + basePath);
    }
    this->basePath = basePath;
}

AssetsManager::~AssetsManager() {}

std::string AssetsManager::readFile(const std::string &fileName) const
{
    fs::path filePath = fs::path(basePath) / fileName;

    if (!fs::exists(filePath))
    {
        throw std::runtime_error("File not found: " + filePath.string());
    }

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filePath.string());
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}