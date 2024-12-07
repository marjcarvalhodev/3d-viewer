#ifndef SHADER_HPP
#define SHADER_HPP

#include <iostream>
#include <GL/glew.h>

enum class ShaderType
{
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER
};

class MyShader
{
private:
    GLuint shaderProgram, vertexShader, fragmentShader, VAO, VBO;
    GLuint compileShader(const char *shaderSource, ShaderType shaderType);

public:
    MyShader();
    ~MyShader();

    bool compileVertexShader(const char *vertexShaderSource);
    bool compileFragmentShader(const char *fragmentShaderSource);

    bool bindShaders();

    void use();

    GLuint getProgramID() const;
};

#endif