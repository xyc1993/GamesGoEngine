#pragma once

#include <vector>

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
	~LightsManager();

	static LightsManager* instance;
	static LightsManager* GetInstance();
	
public:	
	static constexpr int MAX_NUMBER_OF_DIR_LIGHTS = 4;
	static constexpr int MAX_NUMBER_OF_POINT_LIGHTS = 4;
	static constexpr int MAX_NUMBER_OF_SPOT_LIGHTS = 4;
	
	static GLuint AddDirectionalLight(DirectionalLight* directionalLight);
	static GLuint AddPointLight(PointLight* pointLight);
	static GLuint AddSpotLight(SpotLight* spotLight);

	static void SetLightsInShader(const GLuint& shaderProgram);

private:
	DirectionalLight* directionalLight[MAX_NUMBER_OF_DIR_LIGHTS];
	PointLight* pointLights[MAX_NUMBER_OF_POINT_LIGHTS];
	SpotLight* spotLight[MAX_NUMBER_OF_SPOT_LIGHTS];

	int directionalLightsNumber;
	int pointLightsNumber;
	int spotLightsNumber;
};