#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);

	void Update() override;
	void SetLightInShader(const GLuint& shaderProgram) override;

protected:
	std::string GetNumberedShaderProperty() override;

private:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;
};