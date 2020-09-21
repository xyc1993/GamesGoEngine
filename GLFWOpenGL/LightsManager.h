#pragma once

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class LightsManager
{
public:	
	static const int MAX_NUMBER_OF_POINT_LIGHTS = 4;

	LightsManager();

	void AddDirectionalLight(GLuint shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
	void AddPointLight(GLuint shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
						glm::vec3 position, float constant, float linear, float quadratic);
	void AddSpotLight(GLuint shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
						glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic,
						float cutOff, float outerCutOff);

	void SetLightsInShader();
	DirectionalLight* GetDirectionalLight();
	PointLight* GetPointLight(int index);
	SpotLight* GetSpotLight();

private:
	DirectionalLight directionalLight;
	PointLight pointLights[MAX_NUMBER_OF_POINT_LIGHTS];
	SpotLight spotLight;

	int directionalLightsNumber;
	int pointLightsNumber;
	int spotLightsNumber;
};