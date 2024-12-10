#include "mesh.hpp"

MyMesh::MyMesh(const std::vector<float> &vertices) : vertexCount(vertices.size() / 3)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

MyMesh::~MyMesh()
{
    glEnableVertexAttribArray(0);
}

void MyMesh::bind() const
{
    glBindVertexArray(VAO);
}

void MyMesh::unbind() const
{
    glBindVertexArray(0);
}

size_t MyMesh::getVertexCount() const
{
    return vertexCount;
}

const glm::mat4 &MyMesh::getModelMatrix() const
{
    return modelMatrix;
}

void MyMesh::setModelMatrix(const glm::mat4 &matrix)
{
    modelMatrix = matrix;
}
