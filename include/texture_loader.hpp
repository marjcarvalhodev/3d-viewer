#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include "stb_image.h"
#include <iostream>
#include <string>
#include <GL/glew.h>

class TextureLoader
{
private:

public:
    TextureLoader();
    ~TextureLoader();

    GLuint loadTextureFromFile(const std::string &path);

};

#endif

