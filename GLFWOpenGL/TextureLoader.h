#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include "SOIL2/SOIL2.h"

#include <vector>

using namespace std;

class TextureLoader
{
public:
    static GLuint LoadTexture(GLchar* path);
    static GLuint LoadCubemap(vector<const GLchar*> faces);
};