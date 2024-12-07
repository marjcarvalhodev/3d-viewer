#include <SDL2/SDL.h>
#include "window.hpp"
#include "assets_mngr.hpp"
#include "shader.hpp"
#include "mesh.hpp"

void render(MyWindow &window, MyShader &shader, MyMesh &mesh);

int main()
{

    try
    {

        MyWindow window("3D-Viewer", 800, 600);
        if (!window.init())
        {
            return 1;
        }

        glViewport(0, 0, window.getWidth(), window.getHeight());
        glClearColor(0.3f, 0.5f, 0.4f, 1.0f);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        AssetsManager asmn("assets");

        std::string vertexShaderSource = asmn.readFile("basic_shader.vert");
        if (vertexShaderSource.empty())
        {
            throw std::runtime_error("Vertex shader source is empty.");
        }
        if (vertexShaderSource.find("#version") == std::string::npos)
        {
            throw std::runtime_error("Vertex shader source does not contain #version directive.");
        }
        std::cout << "Vertex Shader Source:\n"
                  << vertexShaderSource << std::endl;

        std::string fragmentShaderSource = asmn.readFile("basic_shader.frag");
        if (fragmentShaderSource.empty())
        {
            throw std::runtime_error("Fragment shader source is empty.");
        }
        if (fragmentShaderSource.find("#version") == std::string::npos)
        {
            throw std::runtime_error("Fragment shader source does not contain #version directive.");
        }
        std::cout << "Fragment Shader Source:\n"
                  << fragmentShaderSource << std::endl;

        MyShader shader;
        shader.compileVertexShader(vertexShaderSource.c_str());
        shader.compileFragmentShader(fragmentShaderSource.c_str());
        shader.bindShaders();

        std::vector<float> vertices = {
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f};

        MyMesh mesh(vertices);

        bool running = true;
        SDL_Event event;
        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
            }

            render(window, shader, mesh);
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

void render(MyWindow &window, MyShader &shader, MyMesh &mesh)
{
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();
    mesh.bind();
    glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());
    mesh.unbind();
    window.swapBuffers();
}