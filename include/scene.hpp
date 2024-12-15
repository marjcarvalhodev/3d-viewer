#ifndef SCENE_HPP
#define SCENE_HPP

#include "object.hpp"
#include "camera.hpp"
#include "window.hpp"
#include <unordered_map>
#include <string>

class Scene
{
private:
    std::unordered_map<std::string, std::shared_ptr<Object>> scene_objects;
    MyCamera camera;

public:
    Scene(const MyCamera &camera) : camera(camera) {};
    ~Scene();

    void addSceneObjects(const std::string &key, const std::shared_ptr<Object> &object)
    {
        scene_objects[key] = object;
    }

    std::shared_ptr<Object> getSceneObject(const std::string &key) const
    {
        return scene_objects.at(key);
    }

    void renderScene(MyWindow &window) const;
};

#endif