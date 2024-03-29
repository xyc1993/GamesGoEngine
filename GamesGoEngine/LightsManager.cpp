#include "LightsManager.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace GamesGoEngine
{
	unsigned int LightsManager::AddAmbientLight(AmbientLight* ambientLight)
	{
		if (this->ambientLight != nullptr)
		{
			this->ambientLight->CopyData(*ambientLight);
			return Light::INITIALIZATION_ERROR;
		}
		else
		{
			this->ambientLight = ambientLight;
			return 0;
		}
	}

	unsigned int LightsManager::AddDirectionalLight(DirectionalLight* directionalLight)
	{
		return AddLight(directionalLights, directionalLight, MAX_NUMBER_OF_DIR_LIGHTS);
	}

	unsigned int LightsManager::AddPointLight(PointLight* pointLight)
	{
		return AddLight(pointLights, pointLight, MAX_NUMBER_OF_POINT_LIGHTS);
	}

	unsigned int LightsManager::AddSpotLight(SpotLight* spotLight)
	{
		return AddLight(spotLights, spotLight, MAX_NUMBER_OF_SPOT_LIGHTS);
	}

	unsigned int LightsManager::AddLight(std::vector<Light*>& lightsVector, Light* light, int maxContainerSize)
	{
		if (lightsVector.size() >= maxContainerSize) return Light::INITIALIZATION_ERROR;
		lightsVector.push_back(light);
		return (lightsVector.size() - 1);
	}

	void LightsManager::RemoveAmbientLight(AmbientLight* ambientLight)
	{
		if (this->ambientLight == ambientLight)
		{
			this->ambientLight = nullptr;
		}
	}

	void LightsManager::RemoveDirectionalLight(DirectionalLight* directionalLight)
	{
		RemoveLight(directionalLights, directionalLight);
	}

	void LightsManager::RemovePointLight(PointLight* pointLight)
	{
		RemoveLight(pointLights, pointLight);
	}

	void LightsManager::RemoveSpotLight(SpotLight* spotLight)
	{
		RemoveLight(spotLights, spotLight);
	}

	void LightsManager::RemoveLight(std::vector<Light*>& lightsVector, Light* light)
	{
		for (size_t i = 0; i < lightsVector.size(); i++)
		{
			if (lightsVector[i] == light)
			{
				lightsVector.erase(lightsVector.begin() + i);
			}
		}
	}

	void LightsManager::SetLightsInShader(const unsigned int& shaderProgram)
	{
		glUniform1i(glGetUniformLocation(shaderProgram, "dirLightsNumber"), directionalLights.size());
		for (size_t i = 0; i < directionalLights.size(); i++)
		{
			if (directionalLights[i] != nullptr)
			{
				directionalLights[i]->SetLightInShader(shaderProgram);
			}
		}

		glUniform1i(glGetUniformLocation(shaderProgram, "pointLightsNumber"), pointLights.size());
		for (size_t i = 0; i < pointLights.size(); i++)
		{
			if (pointLights[i] != nullptr)
			{
				pointLights[i]->SetLightInShader(shaderProgram);
			}
		}

		glUniform1i(glGetUniformLocation(shaderProgram, "spotLightsNumber"), spotLights.size());
		for (size_t i = 0; i < spotLights.size(); i++)
		{
			if (spotLights[i] != nullptr)
			{
				spotLights[i]->SetLightInShader(shaderProgram);
			}
		}
	}

	Light* LightsManager::GetAmbientLight() const
	{
		return ambientLight;
	}

	Light* LightsManager::GetDirectionalLight(int lightIndex) const
	{
		return GetLight(directionalLights, lightIndex);
	}

	Light* LightsManager::GetPointLight(int lightIndex) const
	{
		return GetLight(pointLights, lightIndex);
	}

	Light* LightsManager::GetSpotLight(int lightIndex) const
	{
		return GetLight(spotLights, lightIndex);
	}

	Light* LightsManager::GetLight(std::vector<Light*> lightsVector, int lightIndex)
	{
		if (lightIndex >= 0 && lightIndex < lightsVector.size())
		{
			if (lightsVector[lightIndex] != nullptr)
			{
				return lightsVector[lightIndex];
			}
		}
		return nullptr;
	}

	size_t LightsManager::GetAmbientLightsNumber() const
	{
		return ambientLight != nullptr ? 1 : 0;
	}

	size_t LightsManager::GetDirectionalLightsNumber() const
	{
		return directionalLights.size();
	}

	size_t LightsManager::GetPointLightsNumber() const
	{
		return pointLights.size();
	}

	size_t LightsManager::GetSpotLightsNumber() const
	{
		return spotLights.size();
	}

	// TODO: generating this everytime is wasteful, cache it
	std::vector<Light*> LightsManager::GetAllLights() const
	{
		std::vector<Light*> allLights;

		if (ambientLight != nullptr)
		{
			allLights.push_back(ambientLight);
		}

		for (size_t i = 0; i < directionalLights.size(); i++)
		{
			allLights.push_back(directionalLights[i]);
		}

		for (size_t i = 0; i < pointLights.size(); i++)
		{
			allLights.push_back(pointLights[i]);
		}

		for (size_t i = 0; i < spotLights.size(); i++)
		{
			allLights.push_back(spotLights[i]);
		}

		return allLights;
	}
}