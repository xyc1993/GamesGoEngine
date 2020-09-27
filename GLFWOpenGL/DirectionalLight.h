#pragma once

#include "Light.h"

class DirectionalLight : Light
{
public:
	DirectionalLight();
	DirectionalLight(GLuint shaderProgram, GLuint lightNumber, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
	void SetLightInShader() override;

protected:
	std::string GetNumberedShaderProperty() override;

private:
	glm::vec3 direction;
};