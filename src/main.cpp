#include "constants.hpp"
#include "utils.hpp"
#include <SDL2/SDL.h>
#include "window.hpp"
#include "assets_mngr.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

void render(MyWindow &window, MyCamera &camera, MyShader &shader, std::vector<MyMesh> &meshes);
MyCamera getDefaultCamera(const MyWindow &window);
void updateLogic(MyCamera &camera, float accumulatedTime);
void windowResize(SDL_Event &event, MyCamera &camera);

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

        AssetsManager asmn;
        std::string vertexShaderSource = asmn.readFile("basic_shader.vert");
        std::string fragmentShaderSource = asmn.readFile("basic_shader.frag");

        MyShader shader({vertexShaderSource, fragmentShaderSource});

        float tz = -1.0f;
        std::vector<float> vertices = {
            0.0f, 0.5f, tz, // Push triangle back
            -0.5f, -0.5f, tz,
            0.5f, -0.5f, tz};

        MyMesh mesh(vertices);
        meshes.push_back(mesh);

        MyCamera camera = getDefaultCamera(window);

        SDL_Event event;

        bool running = true;

        float lastUpdateTime = SDL_GetTicks();
        float accumulatedTime = 0.0f;
        float frames = 0.0f;

        while (running)
        {
            float currentTime = SDL_GetTicks();
            float deltaTime = currentTime - lastUpdateTime;
            lastUpdateTime = currentTime;

            accumulatedTime += deltaTime;

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }

                static Uint32 lastResizeTime = 0;  // Time of the last resize event
                const Uint32 resizeThrottle = 100; // Minimum time between updates (in ms)

                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    SDL_Event skipEvent;
                    while (SDL_PeepEvents(&skipEvent, 1, SDL_GETEVENT, SDL_WINDOWEVENT, SDL_WINDOWEVENT) > 0)
                    {
                        if (skipEvent.window.event == SDL_WINDOWEVENT_RESIZED)
                            continue;
                    }

                    if (currentTime - lastResizeTime > resizeThrottle)
                    {
                        windowResize(event, camera);
                    }
                }
            }

            while (accumulatedTime >= MS_PER_TICK)
            {
                // updateLogic(camera, accumulatedTime);
                accumulatedTime -= MS_PER_TICK;

                // const glm::mat4 vMat = camera.getViewMatrix();
                // print("View Matrix:\n", glm::to_string(vMat));
            }

            render(window, camera, shader, meshes);
            for (size_t i = 0; i < vertices.size(); i += 3)
            {
                glm::vec4 worldPos(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
                glm::vec4 clipPos = camera.getProjectionMatrix() * camera.getViewMatrix() * worldPos;
                print("World Pos: ", glm::to_string(worldPos), " -> Clip Pos: ", glm::to_string(clipPos));
            }
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        printErr(e);
        return 1;
    }
}

void render(MyWindow &window, MyCamera &camera, MyShader &shader, std::vector<MyMesh> &meshes)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    std::cout << "Projection Matrix:\n"
              << glm::to_string(camera.getProjectionMatrix()) << std::endl;

    return camera;
}

void updateLogic(MyCamera &camera, float accumulatedTime)
{
    float angle = glm::radians(fmod(accumulatedTime * 50.0f, 360.0f));
    // print(angle);
    // glm::vec3 target = glm::vec3(sin(angle), cos(angle), 0.0f);
    // camera.setTarget(target);

    glm::vec3 newPos = glm::vec3(sin(angle) * 30.0f, cos(angle) * 30.0f, 3.0f);
    camera.setPosition(newPos);
}

void windowResize(SDL_Event &event, MyCamera &camera)
{
    int newWidth = event.window.data1;
    int newHeight = event.window.data2;

    glViewport(0, 0, newWidth, newHeight);

    camera.setAspectRatio(static_cast<float>(newWidth) / newHeight);
    camera.updateProjectionMatrix();
}