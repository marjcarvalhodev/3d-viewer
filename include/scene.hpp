#ifndef SCENE_HPP
#define SCENE_HPP

#include "object.hpp"
#include <unordered_map>
#include <string>

class Scene
{
private:
    std::unordered_map<std::string, Object> scene_objects;

public:
    Scene();
    ~Scene();
};

#endif