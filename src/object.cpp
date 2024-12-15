#include "object.hpp"

Object::Object(std::shared_ptr<MyMesh> mesh, const Material &material, std::shared_ptr<MyShader> shader)
    : mesh(mesh), material(material), shader(shader), modelMatrix(glm::mat4(1.0f))
{
}

Object::~Object()
{
}

void Object::render(const glm::mat4 &view, const glm::mat4 &projection)
{
    shader->use();
    shader->updateShader(modelMatrix, view, projection, glm::vec3(10.0));

    // Pass material properties to the shader
    GLuint colorLoc = glGetUniformLocation(shader->getProgramID(), "materialColor");
    glUniform3fv(colorLoc, 1, glm::value_ptr(material.color));

    GLuint shininessLoc = glGetUniformLocation(shader->getProgramID(), "materialShininess");
    glUniform1f(shininessLoc, material.shininess);

    mesh->bind();
    glDrawArrays(GL_TRIANGLES, 0, mesh->getVertexCount());
    mesh->unbind();
}

// eof