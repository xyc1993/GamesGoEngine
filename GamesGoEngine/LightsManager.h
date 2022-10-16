#pragma once

#include <vector>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

/*
 * Centralized lights manager that handles all of the spawned lights
 */
class LightsManager
{
public:
	LightsManager() = default;	

	const int MAX_NUMBER_OF_DIR_LIGHTS = 4;
	const int MAX_NUMBER_OF_POINT_LIGHTS = 4;
	const int MAX_NUMBER_OF_SPOT_LIGHTS = 4;
	
	GLuint AddDirectionalLight(DirectionalLight* directionalLight);
	GLuint AddPointLight(PointLight* pointLight);
	GLuint AddSpotLight(SpotLight* spotLight);

private:
	GLuint AddLight(std::vector<Light*>& lightsVector, Light* light, int maxContainerSize);

public:
	void RemoveDirectionalLight(DirectionalLight* directionalLight);
	void RemovePointLight(PointLight* pointLight);
	void RemoveSpotLight(SpotLight* spotLight);

private:
	void RemoveLight(std::vector<Light*>& lightsVector, Light* light);

public:
	void SetLightsInShader(const GLuint& shaderProgram);

	bool TryGetDirectionalLight(Light& directionalLight, int lightIndex) const;
	bool TryGetPointLight(Light& pointLight, int lightIndex) const;
	bool TryGetSpotLight(Light& spotLight, int lightIndex) const;

private:
	static bool TryGetLight(Light& light, std::vector<Light*> lightsVector, int lightIndex);

public:
	size_t GetDirectionalLightsNumber() const;
	size_t GetPointLightsNumber() const;
	size_t GetSpotLightsNumber() const;

private:
	std::vector<Light*> directionalLights;
	std::vector<Light*> pointLights;
	std::vector<Light*> spotLights;
};