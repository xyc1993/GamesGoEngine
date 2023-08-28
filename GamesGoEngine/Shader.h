#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>

namespace GamesGoEngine
{
    enum class ShaderType
    {
        Vertex,
        Fragment,
        Geometry
    };

    class Shader
    {
    public:
        Shader();
        Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
        Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath);
        void Use() const;
        GLuint GetProgram() const;

    private:
        void CreateProgram(const std::vector<GLuint>& shadersToAttach);
        static bool LoadShader(GLuint& shader, const GLchar* path, ShaderType shaderType);
        static std::string GetShaderTypeName(ShaderType shaderType);
        static GLuint GetShaderInteger(ShaderType shaderType);

        GLuint program;
    };
}