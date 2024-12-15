#include "mesh.hpp"

MyMesh::MyMesh(MeshData meshData) : vertexCount(meshData.vertices.size() / 3)
{
    std::vector<float> vertices = meshData.vertices;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Create and bind normals buffer (if provided)
    if (!meshData.normals.empty())
    {
        std::vector<float> normals = meshData.normals;

        GLuint normalVBO;
        glGenBuffers(1, &normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
    }

    // Create and bind texture coordinates buffer (if provided)
    if (!meshData.texCoords.empty())
    {
        std::vector<float> texCoords = meshData.texCoords;

        GLuint texCoordVBO;
        glGenBuffers(1, &texCoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(2);
    }

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

//