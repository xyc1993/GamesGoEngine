#pragma once

#include <string>
#include <sstream>
#include <vector>

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
        Shader(const char* vertexPath, const char* fragmentPath);
        Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
        void Use() const;
        unsigned int GetProgram() const;

    private:
        void CreateProgram(const std::vector<unsigned int>& shadersToAttach);
        static bool LoadShader(unsigned int& shader, const char* path, ShaderType shaderType);
        static std::string GetShaderTypeName(ShaderType shaderType);
        static unsigned int GetShaderInteger(ShaderType shaderType);

        unsigned int program;
    };
}