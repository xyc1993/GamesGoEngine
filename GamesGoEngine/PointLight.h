#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
	~PointLight();

	void Update() override;
	void SetThisNonNumberedLightInShader(const GLuint& shaderProgram);
	void SetThisLightInShader(const GLuint& shaderProgram) override;
	void SetLightInShader(const GLuint& shaderProgram) override;

private:
	void SetLightInShader(const GLuint& shaderProgram, int lightNumber);

protected:
	std::string GetNumberedShaderProperty(int lightNumber) override;

private:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;
};