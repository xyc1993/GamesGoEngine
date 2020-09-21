#pragma once

#include <string>

#include "Light.h"

class PointLight : Light
{
public:
	PointLight();
	PointLight(GLuint shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
				int lightNumber, glm::vec3 position, float constant, float linear, float quadratic);
	void SetLightInShader() override;
	void SetPosition(glm::vec3 position);
	void SetPosition(float x, float y, float z);

private:
	std::string GetNumberedShaderProperty();

	int lightNumber;

	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;
};