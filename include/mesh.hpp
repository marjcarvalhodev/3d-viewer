#ifndef MESH_HPP
#define MESH_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <vector>

class MyMesh {
private:
    GLuint VAO, VBO;
    size_t vertexCount;

public:

    MyMesh(const std::vector<float>& vertices);
    ~MyMesh();

    void bind() const;
    void unbind() const;
    size_t getVertexCount() const;
};

#endif