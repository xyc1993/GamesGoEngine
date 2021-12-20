#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include "Component.h"

class Light : public Component
{
public:
	// works with correct shader supporting light, forward rendering
	virtual void SetLightInShader(const GLuint& shaderProgram) = 0;
	virtual void Update() override {}

	static constexpr GLuint INITIALIZATION_ERROR = -1;

protected:
	virtual std::string GetNumberedShaderProperty() = 0;

	// light number (index) in shader array
	GLuint lightNumber;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};