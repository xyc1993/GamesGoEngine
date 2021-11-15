#pragma once

#include "Light.h"

class SpotLight : Light
{
public:
	SpotLight();
	SpotLight(GLuint lightNumber, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
				glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic,
				float cutOff, float outerCutOff);
	void SetLightInShader(const GLuint& shaderProgram) override;
	void SetPosition(glm::vec3 position);
	void SetPosition(float x, float y, float z);
	void SetDirection(glm::vec3 direction);
	void SetDirection(float x, float y, float z);

protected:
	std::string GetNumberedShaderProperty() override;

private:
	glm::vec3 position;
	glm::vec3 direction;
	
	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};