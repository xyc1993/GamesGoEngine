#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
    Shader();
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);    
    void Use();
    GLuint GetProgram() const;

private:
    GLuint program;
};