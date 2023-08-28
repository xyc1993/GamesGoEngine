#include "Shader.h"

namespace GamesGoEngine
{
    Shader::Shader()
    {

    }

    Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
    {
        // Load shaders from files
        GLuint vertex, fragment;
        const bool vertexLoadSuccess = LoadShader(vertex, vertexPath, ShaderType::Vertex);
        const bool fragmentLoadSuccess = LoadShader(fragment, fragmentPath, ShaderType::Fragment);

        if (!vertexLoadSuccess || !fragmentLoadSuccess)
        {
            return;
        }

        std::vector<GLuint> shadersToAttach;
        shadersToAttach.push_back(vertex);
        shadersToAttach.push_back(fragment);

        CreateProgram(shadersToAttach);
    }

    Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
    {
        // Load shaders from files
        GLuint vertex, fragment, geometry;
        const bool vertexLoadSuccess = LoadShader(vertex, vertexPath, ShaderType::Vertex);
        const bool fragmentLoadSuccess = LoadShader(fragment, fragmentPath, ShaderType::Fragment);
        const bool geometryLoadSuccess = LoadShader(geometry, geometryPath, ShaderType::Geometry);

        if (!vertexLoadSuccess || !fragmentLoadSuccess || !geometryLoadSuccess)
        {
            return;
        }

        std::vector<GLuint> shadersToAttach;
        shadersToAttach.push_back(vertex);
        shadersToAttach.push_back(geometry);
        shadersToAttach.push_back(fragment);

        CreateProgram(shadersToAttach);
    }

    void Shader::Use() const
    {
        glUseProgram(this->program);
    }

    GLuint Shader::GetProgram() const
    {
        return program;
    }

    void Shader::CreateProgram(const std::vector<GLuint>& shadersToAttach)
    {
        this->program = glCreateProgram();
        for (int i = 0; i < shadersToAttach.size(); i++)
        {
            glAttachShader(this->program, shadersToAttach[i]);
        }
        glLinkProgram(this->program);

        // Print linking errors if any
        GLint success;
        GLchar infoLog[512];

        glGetProgramiv(this->program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // Delete the shaders as they're linked into our program now and no longer necessary
        for (int i = 0; i < shadersToAttach.size(); i++)
        {
            glDeleteShader(shadersToAttach[i]);
        }
    }

    bool Shader::LoadShader(GLuint& shader, const GLchar* path, ShaderType shaderType)
    {
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string shaderFileCode;
        std::ifstream shaderFile;

        // ensures ifstream objects can throw exceptions:
        shaderFile.exceptions(std::ifstream::badbit);
        try
        {
            // Open files
            shaderFile.open(path);
            std::stringstream shaderStream;
            // Read file's buffer contents into streams
            shaderStream << shaderFile.rdbuf();
            // close file handlers
            shaderFile.close();
            // Convert stream into string
            shaderFileCode = shaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            printf("ERROR::SHADER::%s::FILE_NOT_SUCCESSFULLY_READ", GetShaderTypeName(shaderType).c_str());
            return false;
        }
        const GLchar* shaderCode = shaderFileCode.c_str();

        // 2. Compile shader
        GLint success;
        GLchar infoLog[512];

        shader = glCreateShader(GetShaderInteger(shaderType));
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);

        // Print compile errors if any
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            printf("ERROR::SHADER::%s::FILE_NOT_SUCCESSFULLY_READ\n%s\n", GetShaderTypeName(shaderType).c_str(), infoLog);
            return false;
        }

        return true;
    }

    std::string Shader::GetShaderTypeName(ShaderType shaderType)
    {
        switch (shaderType)
        {
        case ShaderType::Vertex: return "VERTEX";
        case ShaderType::Fragment: return "FRAGMENT";
        case ShaderType::Geometry: return "GEOMETRY";
        default: return "";
        }
    }

    GLuint Shader::GetShaderInteger(ShaderType shaderType)
    {
        switch (shaderType)
        {
        case ShaderType::Vertex: return GL_VERTEX_SHADER;
        case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
        case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
        default: return 0;
        }
    }
}