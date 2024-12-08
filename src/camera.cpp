#include "camera.hpp"

Camera::Camera(const glm::vec3 &pos, const glm::vec3 &tgt, const glm::vec3 &upDir, float aspect)
    : position(pos), target(tgt), up(upDir), fov(45.0f), aspectRatio(aspect), nearPlane(0.1f), farPlane(100.0f)
{
    updateViewMatrix();
    updateProjectionMatrix();
}

Camera::~Camera()
{
}

void Camera::updateViewMatrix()
{
    viewMatrix = glm::lookAt(position, target, up);
}

void Camera::updateProjectionMatrix()
{
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

const glm::mat4 &Camera::getViewMatrix() const
{
    return viewMatrix;
}

const glm::mat4 &Camera::getProjectionMatrix() const
{
    return projectionMatrix;
}