#include "constants.hpp"
#include "utils.hpp"
#include <SDL2/SDL.h>
#include "window.hpp"
#include "camera.hpp"
#include "assets_manager.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "mesh_loader.hpp"
#include "object.hpp"
#include "scene.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

MyCamera getDefaultCamera(const MyWindow &window);
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

        std::shared_ptr<Object> shiny_ball = std::make_shared<Object>(ballMesh, mat, phong_shader);
        std::shared_ptr<Object> shiny_diamond = std::make_shared<Object>(diamondMesh, mat, phong_shader);

        Scene main_scene(camera);
        main_scene.addSceneObjects("shiny_ball", shiny_ball);
        main_scene.addSceneObjects("shiny_ball", shiny_diamond);

        while (running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
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
    glm::vec3 position = {0.0, 6.0, 15.0};
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

// eof