#pragma once

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

/*
 * Centralized lights manager
 * Singleton, there can be only one lights manager that handles all of the spawned lights
 */
class LightsManager
{
private:
	LightsManager();

	static LightsManager* instance;
	static LightsManager* GetInstance();
	
public:	
	static const int MAX_NUMBER_OF_DIR_LIGHTS = 4;
	static const int MAX_NUMBER_OF_POINT_LIGHTS = 4;
	static const int MAX_NUMBER_OF_SPOT_LIGHTS = 4;
	
	static void AddDirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
	static void AddPointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
						glm::vec3 position, float constant, float linear, float quadratic);
	static void AddSpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
						glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic,
						float cutOff, float outerCutOff);

	static void SetLightsInShader(const GLuint& shaderProgram);

	// TO DO: remove directo access to lights via light manager, in the future lights should be game object's components
	static DirectionalLight* GetDirectionalLight(int index);
	static PointLight* GetPointLight(int index);
	static SpotLight* GetSpotLight(int index);

private:
	DirectionalLight directionalLight[MAX_NUMBER_OF_DIR_LIGHTS];
	PointLight pointLights[MAX_NUMBER_OF_POINT_LIGHTS];
	SpotLight spotLight[MAX_NUMBER_OF_SPOT_LIGHTS];

	int directionalLightsNumber;
	int pointLightsNumber;
	int spotLightsNumber;
};