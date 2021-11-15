#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class Light
{
public:
	// works with correct shader supporting light, forward rendering
	virtual void SetLightInShader(const GLuint& shaderProgram) = 0;

protected:
	virtual std::string GetNumberedShaderProperty() = 0;

	// light number (index) in shader array
	GLuint lightNumber;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};