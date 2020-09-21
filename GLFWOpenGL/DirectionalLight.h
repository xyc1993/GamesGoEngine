#pragma once

#include "Light.h"

class DirectionalLight : Light
{
public:
	DirectionalLight(GLuint shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
	void SetLightInShader() override;

private:
	glm::vec3 direction;
};