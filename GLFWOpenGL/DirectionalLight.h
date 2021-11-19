#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	void Update() override;
	void SetLightInShader(const GLuint& shaderProgram) override;

protected:
	std::string GetNumberedShaderProperty() override;

private:
	glm::vec3 direction;
};