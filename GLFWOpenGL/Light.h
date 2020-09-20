#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	//works with correct shader supporting light, forward rendering
	virtual void SetLightInShader() = 0;

protected:
	GLuint shaderProgram;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};