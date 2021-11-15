#pragma once

#include "Light.h"

class PointLight : Light
{
public:
	PointLight();
	PointLight(GLuint lightNumber, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
				glm::vec3 position, float constant, float linear, float quadratic);
	void SetLightInShader(const GLuint& shaderProgram) override;
	void SetPosition(glm::vec3 position);
	void SetPosition(float x, float y, float z);

protected:
	std::string GetNumberedShaderProperty() override;

private:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;
};