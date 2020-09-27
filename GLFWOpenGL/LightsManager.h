#pragma once

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class LightsManager
{
public:	
	static const int MAX_NUMBER_OF_DIR_LIGHTS = 4;
	static const int MAX_NUMBER_OF_POINT_LIGHTS = 4;
	static const int MAX_NUMBER_OF_SPOT_LIGHTS = 4;

	LightsManager(GLuint shaderProgram);

	void AddDirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
	void AddPointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
						glm::vec3 position, float constant, float linear, float quadratic);
	void AddSpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
						glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic,
						float cutOff, float outerCutOff);

	void SetLightsInShader();
	DirectionalLight* GetDirectionalLight(int index);
	PointLight* GetPointLight(int index);
	SpotLight* GetSpotLight(int index);

private:
	DirectionalLight directionalLight[MAX_NUMBER_OF_DIR_LIGHTS];
	PointLight pointLights[MAX_NUMBER_OF_POINT_LIGHTS];
	SpotLight spotLight[MAX_NUMBER_OF_SPOT_LIGHTS];

	GLuint shaderProgram;

	int directionalLightsNumber;
	int pointLightsNumber;
	int spotLightsNumber;
};