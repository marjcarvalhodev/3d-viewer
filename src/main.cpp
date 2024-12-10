#include "constants.hpp"
#include "utils.hpp"
#include <SDL2/SDL.h>
#include "window.hpp"
#include "assets_mngr.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "tiny_obj_loader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

void render(MyWindow &window, MyCamera &camera, MyShader &shader, std::vector<MyMesh> &meshes);
MyCamera getDefaultCamera(const MyWindow &window);
void updateLogic(MyCamera &camera, float deltaTime);
void windowResize(SDL_Event &event, MyCamera &camera);
std::vector<float> loadModel(const std::string &objPath);

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

        AssetsManager asmn("assets");
        std::string vertexShaderSource = asmn.readFile("shaders/phong/vertex.glsl");
        std::string fragmentShaderSource = asmn.readFile("shaders/phong/fragment.glsl");

        ShaderSources sources = {vertexShaderSource, fragmentShaderSource};

        MyShader shader(sources);

        float tz = 0.0f;
        std::vector<float> vertices = {
            0.0f, 0.5f, tz,
            -0.5f, -0.5f, tz,
            0.5f, -0.5f, tz};

        std::string tubeModelSource = asmn.readFile("models/tube/tube.obj");
        print(tubeModelSource);

        // try
        // {
        //     std::vector<float> vertices_3D = loadModel(tubeModelSource);

        //     MyMesh mesh(vertices_3D);
        //     meshes.push_back(mesh);
        // }
        // catch (const std::exception &e)
        // {
        //     printErr(e);
        //     return 1;
        // }

        MyMesh mesh(vertices);
        meshes.push_back(mesh);

        MyCamera camera = getDefaultCamera(window);

        SDL_Event event;

        bool running = true;

        float lastUpdateTime = SDL_GetTicks();
        float accumulatedTime = 0.0f;

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

                static Uint32 lastResizeTime = 0;
                const Uint32 resizeThrottle = 100;

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
                // updateLogic(camera, deltaTime);
                accumulatedTime -= MS_PER_TICK;
            }

            const float maxFPS = 60.0f;
            const float frameDelay = 1000.0f / maxFPS;

            float frameStart = SDL_GetTicks();
            render(window, camera, shader, meshes);
            float frameTime = SDL_GetTicks() - frameStart;

            if (frameTime < frameDelay)
            {
                SDL_Delay((frameDelay - frameTime));
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

    for (auto &mesh : meshes)
    {
        shader.updateShader(mesh.getModelMatrix(), camera.getViewMatrix(), camera.getProjectionMatrix());
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

void updateLogic(MyCamera &camera, float deltaTime)
{
    static float accumulatedLogicTime = 0.0f;
    accumulatedLogicTime += deltaTime;

    float radius = 3.0f;
    float angle = glm::radians(fmod(accumulatedLogicTime * 30.0f, 360.0f));
    glm::vec3 newPos = glm::vec3(sin(angle) * radius, 1.0f, cos(angle) * radius);

    static glm::vec3 lastPos = camera.getPosition();

    float logicTimeStep = 1000.0f / 30.0f;
    float alpha = deltaTime / logicTimeStep;
    alpha = glm::clamp(alpha, 0.0f, 1.0f);

    glm::vec3 interpolatedPos = interpolate(lastPos, newPos, alpha);

    static float logTimer = 0.0f;
    logTimer += deltaTime;

    if (logTimer >= 1000.0f)
    {
        print("New Pos: ", newPos.x, ", ", newPos.y, ", ", newPos.z);
        print("Interpolated Pos: ", interpolatedPos.x, ", ", interpolatedPos.y, ", ", interpolatedPos.z);
        print("Alpha: ", alpha);
        print("New Pos Radius: ", glm::length(newPos));

        logTimer = 0.0f;
    }

    camera.setPosition(interpolatedPos);

    if (glm::distance(interpolatedPos, newPos) < 0.01f)
    {
        lastPos = newPos;
    }
}

void windowResize(SDL_Event &event, MyCamera &camera)
{
    int newWidth = event.window.data1;
    int newHeight = event.window.data2;

    glViewport(0, 0, newWidth, newHeight);

    camera.setAspectRatio(static_cast<float>(newWidth) / newHeight);
    camera.updateProjectionMatrix();
}

// std::vector<float> loadModel(const std::string &objPath)
// {
//     tinyobj::attrib_t attrib;
//     std::vector<tinyobj::shape_t> shapes;
//     std::vector<tinyobj::material_t> materials;
//     std::string warn;
//     std::string err;

//     print(objPath);

//     bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objPath.c_str());

//     if (!success)
//     {
//         throw std::runtime_error("Failed to load OBJ file: " + objPath + "\n" + err);
//     }

//     if (!warn.empty())
//     {
//         std::cerr << "TinyObjLoader Warning: " << warn << std::endl;
//     }

//     std::cout << "Loaded OBJ with " << attrib.vertices.size() / 3 << " vertices." << std::endl;

//     std::vector<float> vertices;

//     for (const auto &shape : shapes)
//     {
//         for (const auto &index : shape.mesh.indices)
//         {
//             float vx = attrib.vertices[3 * index.vertex_index + 0];
//             float vy = attrib.vertices[3 * index.vertex_index + 1];
//             float vz = attrib.vertices[3 * index.vertex_index + 2];

//             vertices.push_back(vx);
//             vertices.push_back(vy);
//             vertices.push_back(vz);
//         }
//     }

//     return vertices;
// }

// eof