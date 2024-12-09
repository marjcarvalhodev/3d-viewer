#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyCamera
{
private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

public:
    MyCamera(const glm::vec3 &pos, const glm::vec3 &tgt, const glm::vec3 &upDir, float aspect);

    void updateViewMatrix();

    void updateProjectionMatrix();

    const glm::mat4 &getViewMatrix() const;

    const glm::mat4 &getProjectionMatrix() const;

    void setPosition(glm::vec3 newValue);
    void setTarget(glm::vec3 newValue);
    void setAspectRatio(float newValue);
};

#endif