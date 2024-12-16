#include "scene.hpp"

MyScene::MyScene(const MyCamera &camera) : camera(camera)
{
}

MyScene::~MyScene()
{
}

void MyScene::animateObject(std::shared_ptr<MyObject> object, float deltaTime)
{
    float angle = glm::radians(30.0f * deltaTime); // Rotate by 30 degrees per second
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    object->spin(rotation);
}

void MyScene::renderScene(MyWindow &window) const
{
    window.clearBuffers();

    for (const auto &[key, object] : scene_objects)
    {
        object->render(camera.getViewMatrix(), camera.getProjectionMatrix());
    }

    window.swapBuffers();
}