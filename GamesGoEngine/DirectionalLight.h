#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~DirectionalLight();

	void Update() override;
	void SetThisLightInShader(const GLuint& shaderProgram) override;
	void SetLightInShader(const GLuint& shaderProgram) override;

private:
	void SetLightInShader(const GLuint& shaderProgram, int lightNumber);

protected:
	std::string GetNumberedShaderProperty(int lightNumber) override;

private:
	glm::vec3 direction;
};