#include "window.hpp"

MyWindow::MyWindow(const std::string &title, int width, int height)
    : window(nullptr), context(nullptr), width(width), height(height), title(title), initialized(false) {}

MyWindow::~MyWindow()
{
    if (context)
    {
        SDL_GL_DeleteContext(context);
        context = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

int MyWindow::getWidth() const
{
    return width;
}

int MyWindow::getHeight() const
{
    return height;
}

bool MyWindow::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create SDL window
    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL);

    if (!window)
    {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // Create OpenGL context
    context = SDL_GL_CreateContext(window);
    if (!context)
    {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        cleanUp();
        return false;
    }

    // Initialize GLEW or other extensions
    glewExperimental = GL_TRUE;
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewInitResult) << std::endl;
        return false;
    }

    // Set the initialized flag
    initialized = true;
    return true;
}

void MyWindow::swapBuffers()
{
    if (initialized)
    {
        SDL_GL_SwapWindow(window);
    }
}

bool MyWindow::shouldClose()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return true;
        }
    }
    return false;
}

void MyWindow::cleanUp()
{
    if (context)
    {
        SDL_GL_DeleteContext(context);
        context = nullptr;
    }
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}