#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "mesh.hpp"
#include "shader.hpp"
#include <memory>

class MyObject
{
private:
    std::shared_ptr<MyMesh> mesh;
    Material material;
    std::shared_ptr<MyShader> shader;
    glm::mat4 modelMatrix;

public:
    bool isTransparent;

    MyObject(
        std::shared_ptr<MyMesh> mesh, const Material &material,
        std::shared_ptr<MyShader> shader, bool isTransparent = false);

    ~MyObject();

    void render(const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &cameraPos);

    void setModelMatrix(const glm::mat4 &matrix) { modelMatrix = matrix; }

    glm::vec3 getPosition() const { return modelMatrix[3]; }

    void repositionObject(glm::vec3 newPosition);

    void spin(glm::mat4 rotationMatrix) { setModelMatrix(rotationMatrix * modelMatrix); }
};

#endif