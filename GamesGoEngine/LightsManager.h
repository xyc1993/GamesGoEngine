#pragma once

#include <vector>

#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace GamesGoEngine
{
	/*
 * Centralized lights manager that handles all of the spawned lights
 */
	class LightsManager
	{
	public:
		LightsManager() = default;

		const int MAX_NUMBER_OF_AMB_LIGHTS = 1;
		const int MAX_NUMBER_OF_DIR_LIGHTS = 4;
		const int MAX_NUMBER_OF_POINT_LIGHTS = 4;
		const int MAX_NUMBER_OF_SPOT_LIGHTS = 4;

		unsigned int AddAmbientLight(AmbientLight* ambientLight);
		unsigned int AddDirectionalLight(DirectionalLight* directionalLight);
		unsigned int AddPointLight(PointLight* pointLight);
		unsigned int AddSpotLight(SpotLight* spotLight);

	private:
		unsigned int AddLight(std::vector<Light*>& lightsVector, Light* light, int maxContainerSize);

	public:
		void RemoveAmbientLight(AmbientLight* ambientLight);
		void RemoveDirectionalLight(DirectionalLight* directionalLight);
		void RemovePointLight(PointLight* pointLight);
		void RemoveSpotLight(SpotLight* spotLight);

	private:
		void RemoveLight(std::vector<Light*>& lightsVector, Light* light);

	public:
		void SetLightsInShader(const unsigned int& shaderProgram);

		Light* GetAmbientLight() const;
		Light* GetDirectionalLight(int lightIndex) const;
		Light* GetPointLight(int lightIndex) const;
		Light* GetSpotLight(int lightIndex) const;

	private:
		static Light* GetLight(std::vector<Light*> lightsVector, int lightIndex);

	public:
		size_t GetAmbientLightsNumber() const;
		size_t GetDirectionalLightsNumber() const;
		size_t GetPointLightsNumber() const;
		size_t GetSpotLightsNumber() const;
		std::vector<Light*> GetAllLights() const;

	private:
		AmbientLight* ambientLight;
		std::vector<Light*> directionalLights;
		std::vector<Light*> pointLights;
		std::vector<Light*> spotLights;
	};
}