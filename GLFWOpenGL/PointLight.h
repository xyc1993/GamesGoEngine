#pragma once

#include <string>

#include "Light.h"

class PointLight : Light
{
public:
	PointLight(GLuint shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
				int lightNumber, glm::vec3 position, float constant, float linear, float quadratic);
	void SetLightInShader() override;

	const int MAX_NUMBER_OF_POINT_LIGHTS = 4;

private:
	std::string GetNumberedShaderProperty();

	int lightNumber;

	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
};