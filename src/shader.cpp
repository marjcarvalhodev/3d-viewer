#include "shader.hpp"

MyShader::MyShader()
    : shaderProgram(0), vertexShader(0), fragmentShader(0) {}

MyShader::~MyShader()
{
    if (shaderProgram)
    {
        glDeleteProgram(shaderProgram);
    }
    if (vertexShader)
    {
        glDeleteShader(vertexShader);
    }
    if (fragmentShader)
    {
        glDeleteShader(fragmentShader);
    }
}

GLuint MyShader::compileShader(const char *shaderSource, ShaderType shaderType)
{
    GLuint shader = glCreateShader(static_cast<GLenum>(shaderType));
    if (shader == 0)
    {
        throw std::runtime_error("Failed to create shader.");
    }

    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool MyShader::compileVertexShader(const char *vertexShaderSource)
{
    vertexShader = compileShader(vertexShaderSource, ShaderType::Vertex);
    return vertexShader != 0;
}

bool MyShader::compileFragmentShader(const char *fragmentShaderSource)
{
    fragmentShader = compileShader(fragmentShaderSource, ShaderType::Fragment);
    return fragmentShader != 0;
}

bool MyShader::bindShaders()
{
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader Program linking failed: " << infoLog << std::endl;
        return false; // Indicate failure
    }

    // Delete shaders after linking (no longer needed)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true; // Indicate success
}

void MyShader::use()
{
    glUseProgram(shaderProgram);
}

GLuint MyShader::getProgramID() const
{
    return shaderProgram;
}