#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include "SOIL2/SOIL2.h"

#include <vector>

class TextureLoader
{
public:
    static GLuint LoadTexture(GLchar* path);
    static GLuint LoadCubemap(std::vector<const GLchar*> faces);
};