#include "constants.hpp"
#include <SDL2/SDL.h>
#include "window.hpp"
#include "assets_mngr.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "camera.hpp"

void render(MyWindow &window, MyCamera &camera, MyShader &shader, std::vector<MyMesh> &meshes);
MyCamera getDefaultCamera(const MyWindow &window);
// void updateLogic(MyCamera &camera, float accumulatedTime);
// void windowResize(SDL_Event &event, MyCamera &camera);

int main()
{
    std::vector<MyMesh> meshes;

    try
    {
        MyWindow window("3D-Viewer", DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT, NIGHT_CLEAR_COLOR);
        if (!window.init())
        {
            return 1;
        }

        glViewport(0, 0, window.getWidth(), window.getHeight());
        window.setClearColor();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        AssetsManager asmn;
        std::string vertexShaderSource = asmn.readFile("basic_shader.vert");
        std::string fragmentShaderSource = asmn.readFile("basic_shader.frag");

        MyShader shader({vertexShaderSource, fragmentShaderSource});

        std::vector<float> vertices = {
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f};

        MyMesh mesh(vertices);
        meshes.push_back(mesh);

        MyCamera camera = getDefaultCamera(window);

        SDL_Event event;

        bool running = true;

        // float lastUpdateTime = SDL_GetTicks();
        // float accumulatedTime = 0.0f;
        // float frames = 0.0f;

        while (running)
        {
            // float currentTime = SDL_GetTicks();
            // float deltaTime = currentTime - lastUpdateTime;
            // lastUpdateTime = currentTime;

            // accumulatedTime += deltaTime;

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }

                // if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
                // {
                //     windowResize(event, camera);
                // }
            }

            // while (accumulatedTime >= MS_PER_TICK)
            // {
            //     updateLogic(camera, accumulatedTime);
            //     accumulatedTime -= MS_PER_TICK;
            // }
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

void render(MyWindow &window, MyCamera &camera, MyShader &shader, std::vector<MyMesh> &meshes)
{
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    shader.updateShader(camera.getViewMatrix(), camera.getProjectionMatrix());

    for (auto &mesh : meshes)
    {
        mesh.bind();
        glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());
        mesh.unbind();
    }

    window.swapBuffers();
}

MyCamera getDefaultCamera(const MyWindow &window)
{
    glm::vec3 position = {0.0, 0.0, 3.0};
    glm::vec3 target = {0.0, 0.0, 0.0};
    glm::vec3 upDir = {0.0, 1.0, 0.0};
    float aspectRatio = static_cast<float>(window.getWidth()) / window.getHeight();

    MyCamera camera(position, target, upDir, aspectRatio);
    return camera;
}

// void updateLogic(MyCamera &camera, float accumulatedTime)
// {
//     float angle = 180.0;
//     camera.setTarget(glm::vec3(sin(angle), cos(angle), 0.0));
// }

// void windowResize(SDL_Event &event, MyCamera &camera)
// {
//     int newWidth = event.window.data1;
//     int newHeight = event.window.data2;

//     glViewport(0, 0, newWidth, newHeight); // Update OpenGL viewport

//     camera.setAspectRatio(static_cast<float>(newWidth) / newHeight); // Update aspect ratio
//     camera.updateProjectionMatrix();                                 // Recalculate the projection matrix
// }