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
	LightsManager() = default;

	static LightsManager* instance;
	static LightsManager* GetInstance();
	
public:	
	static constexpr int MAX_NUMBER_OF_DIR_LIGHTS = 4;
	static constexpr int MAX_NUMBER_OF_POINT_LIGHTS = 4;
	static constexpr int MAX_NUMBER_OF_SPOT_LIGHTS = 4;
	
	static GLuint AddDirectionalLight(DirectionalLight* directionalLight);
	static GLuint AddPointLight(PointLight* pointLight);
	static GLuint AddSpotLight(SpotLight* spotLight);

private:
	static GLuint AddLight(std::vector<Light*>& lightsVector, Light* light, int maxContainerSize);

public:
	static void RemoveDirectionalLight(DirectionalLight* directionalLight);
	static void RemovePointLight(PointLight* pointLight);
	static void RemoveSpotLight(SpotLight* spotLight);

private:
	static void RemoveLight(std::vector<Light*>& lightsVector, Light* light);

public:
	static void SetLightsInShader(const GLuint& shaderProgram);

private:
	std::vector<Light*> directionalLights;
	std::vector<Light*> pointLights;
	std::vector<Light*> spotLights;
};