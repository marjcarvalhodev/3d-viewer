#include "assets_manager.hpp"

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

void AssetsManager::loadModel(const std::string &key, const std::string &modelName)
{
    std::string path = basePath + "/models/" + modelName + "/" + modelName + ".obj";
    models[key] = std::make_shared<MyMesh>(MyMesh(MeshLoader::loadModel(path)));
}

void AssetsManager::loadShader(const std::string &key, const std::string &shaderName)
{
    std::string vertexShaderSource = readFile("shaders/" + shaderName + "/vertex.glsl");
    std::string fragmentShaderSource = readFile("shaders/" + shaderName + "/fragment.glsl");

    ShaderSources sources = {vertexShaderSource, fragmentShaderSource};
    shaders[key] = std::make_shared<MyShader>(sources);
}

// eof