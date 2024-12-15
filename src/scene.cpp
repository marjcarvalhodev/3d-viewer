#include "scene.hpp"

Scene::Scene(const MyCamera &camera) : camera(camera)
{
}

Scene::~Scene()
{
}

void Scene::renderScene(MyWindow &window) const
{
    window.clearBuffers();

    for (const auto &[key, object] : scene_objects)
    {
        object->render(camera.getViewMatrix(), camera.getProjectionMatrix());
    }

    window.swapBuffers();
}