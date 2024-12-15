#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "mesh.hpp"
#include "shader.hpp"
#include <memory>

class Object
{
private:
    std::shared_ptr<MyMesh> mesh;
    Material material;
    std::shared_ptr<MyShader> shader;
    glm::mat4 modelMatrix;

public:
    Object(std::shared_ptr<MyMesh> mesh, const Material &material, std::shared_ptr<MyShader> shader);

    ~Object();

    void render(const glm::mat4 &view, const glm::mat4 &projection);

    void setModelMatrix(const glm::mat4 &matrix) { modelMatrix = matrix; }

};

#endif