#include "Skybox.h"
#include "Primitives.h"
#include "TextureLoader.h"
#include <glm/gtc/type_ptr.hpp>

Skybox::Skybox()
{
	std::vector<const GLchar*> faces;
	faces.push_back("res/images/skybox/right.tga");
	faces.push_back("res/images/skybox/left.tga");
	faces.push_back("res/images/skybox/top.tga");
	faces.push_back("res/images/skybox/bottom.tga");
	faces.push_back("res/images/skybox/back.tga");
	faces.push_back("res/images/skybox/front.tga");

	GLchar* vert = (GLchar*)"res/shaders/skybox.vert";
	GLchar* frag = (GLchar*)"res/shaders/skybox.frag";

	InitSkybox(faces, vert, frag);
}

Skybox::Skybox(const std::vector<const GLchar*> &faces, const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath)
{
	InitSkybox(faces, vertexShaderPath, fragmentShaderPath);
}

void Skybox::InitSkybox(const std::vector<const GLchar*>& faces, const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath)
{
	skyboxShader = Shader(vertexShaderPath, fragmentShaderPath);

	GLuint skyyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Primitives::skyboxVerticesCube), &Primitives::skyboxVerticesCube, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glBindVertexArray(0);

	cubemapTexture = TextureLoader::LoadCubemap(faces);
}

void Skybox::Draw(glm::mat4 view, glm::mat4 projection)
{
	glDepthFunc(GL_LEQUAL);

	skyboxShader.Use();

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(skyboxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}