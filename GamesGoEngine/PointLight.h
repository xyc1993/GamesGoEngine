#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
	~PointLight();

	void Update() override;
	void SetLightInShader(const GLuint& shaderProgram, bool isNumberedLight = true, 
		bool overrideThisLightNumber = false, int thisLightNumberOverride = 0, 
		bool overrideLightsNumber = false, int lightsNumberOverride = 0) override;

protected:
	std::string GetNumberedShaderProperty(int lightNumber) override;

private:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;
};