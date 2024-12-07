#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>

class MyWindow
{
private:
    SDL_Window *window;
    SDL_GLContext context;
    int width, height;
    std::string title;
    bool initialized;

public:
    MyWindow(const std::string &title, int width, int height);

    ~MyWindow();

    bool init();

    void swapBuffers();

    bool shouldClose();

    void cleanUp();

    int getWidth() const;
    int getHeight() const;
};

#endif