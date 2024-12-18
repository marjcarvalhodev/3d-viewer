#include "constants.hpp"
#include "utils.hpp"
#include <SDL2/SDL.h>
#include "window.hpp"
#include "camera.hpp"
#include "assets_manager.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "object.hpp"
#include "scene.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

void windowResize(SDL_Event &event, MyCamera &camera);
MyCamera getDefaultCamera(const MyWindow &window);
void handleInput(SDL_Event &event, bool &running);

int main()
{
    try
    {
        MyWindow window("3D-Viewer", DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT, NIGHT_CLEAR_COLOR);
        if (!window.init())
        {
            return 1;
        }
        MyCamera camera = getDefaultCamera(window);
        SDL_Event event;
        bool running = true;

        AssetsManager assetsManager("assets");

        assetsManager.loadModel("ballModel", "ball");
        assetsManager.loadModel("diamondModel", "diamond");
        assetsManager.loadShader("phongShader", "phong");

        std::shared_ptr<MyMesh> ballMesh = assetsManager.getModel("ballModel");
        std::shared_ptr<MyMesh> diamondMesh = assetsManager.getModel("diamondModel");
        std::shared_ptr<MyShader> phong_shader = assetsManager.getShader("phongShader");

        Material mat = {{0.5, 0.3, 0.36}, 50.0};

        std::shared_ptr<MyObject> shiny_ball_1 = std::make_shared<MyObject>(ballMesh, mat, phong_shader);
        std::shared_ptr<MyObject> shiny_ball_2 = std::make_shared<MyObject>(ballMesh, mat, phong_shader);

        std::shared_ptr<MyObject> shiny_diamond_1 = std::make_shared<MyObject>(diamondMesh, mat, phong_shader);
        std::shared_ptr<MyObject> shiny_diamond_2 = std::make_shared<MyObject>(diamondMesh, mat, phong_shader);

        MyScene main_scene(camera);

        shiny_ball_1->repositionObject({-2.0, 0.0, 2.0});
        shiny_ball_2->repositionObject({-2.0, 0.0, -2.0});
        shiny_diamond_1->repositionObject({2.0, 0.0, 2.0});
        shiny_diamond_2->repositionObject({2.0, 0.0, -2.0});

        main_scene.addSceneObjects("shiny_ball_1", shiny_ball_1);
        main_scene.addSceneObjects("shiny_ball_2", shiny_ball_2);
        main_scene.addSceneObjects("shiny_diamond_1", shiny_diamond_1);
        main_scene.addSceneObjects("shiny_diamond_2", shiny_diamond_2);

        float lastTime = SDL_GetTicks() / 1000.0f; // Initialize lastTime in seconds

        while (running)
        {
            float currentTime = SDL_GetTicks() / 1000.0f; // Current time in seconds
            float deltaTime = currentTime - lastTime;     // Time elapsed since last frame
            lastTime = currentTime;

            handleInput(event, running);

            for (const auto &[key, object] : main_scene.getAllSceneObjects())
            {
                main_scene.animateObject(object, deltaTime);
            }

            main_scene.renderScene(window);
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        printErr(e);
        return 1;
    }
}

MyCamera getDefaultCamera(const MyWindow &window)
{
    glm::vec3 position = {0.0, 10.0, 15.0};
    glm::vec3 target = {0.0, 0.0, 0.0};
    glm::vec3 upDir = {0.0, 1.0, 0.0};
    float aspectRatio = static_cast<float>(window.getWidth()) / window.getHeight();

    MyCamera camera(position, target, upDir, aspectRatio);
    return camera;
}

void windowResize(SDL_Event &event, MyCamera &camera)
{
    int newWidth = event.window.data1;
    int newHeight = event.window.data2;

    glViewport(0, 0, newWidth, newHeight);

    camera.setAspectRatio(static_cast<float>(newWidth) / newHeight);
    camera.updateProjectionMatrix();
}

void handleInput(SDL_Event &event, bool &running)
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
    }
}

// eof