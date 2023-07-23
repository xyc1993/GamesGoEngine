#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include "Component.h"

class Light : public Component
{
public:
	// used when setting only this light in shader
	virtual void SetThisLightInShader(const GLuint& shaderProgram) = 0;
	// used when setting several lights in shader at once
	virtual void SetLightInShader(const GLuint& shaderProgram) = 0;
	virtual void Update() override {}

	static constexpr GLuint INITIALIZATION_ERROR = -1;

protected:
	virtual std::string GetNumberedShaderProperty(int lightNumber) = 0;

	// light number (index) in shader array
	GLuint lightNumber;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};