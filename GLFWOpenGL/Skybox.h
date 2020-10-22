#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"

class Skybox
{
public:
	Skybox();
	Skybox(const std::vector<const GLchar*>& faces, const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath);
	void Draw(glm::mat4 view, glm::mat4 projection);

private:
	Shader skyboxShader;
	GLuint cubemapTexture;
	GLuint skyboxVAO;

	void InitSkybox(const std::vector<const GLchar*>& faces, const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath);
};