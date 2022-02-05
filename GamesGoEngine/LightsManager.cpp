#include "LightsManager.h"

GLuint LightsManager::AddDirectionalLight(DirectionalLight* directionalLight)
{
	return AddLight(directionalLights, directionalLight, MAX_NUMBER_OF_DIR_LIGHTS);
}

GLuint LightsManager::AddPointLight(PointLight* pointLight)
{
	return AddLight(pointLights, pointLight, MAX_NUMBER_OF_POINT_LIGHTS);
}

GLuint LightsManager::AddSpotLight(SpotLight* spotLight)
{
	return AddLight(spotLights, spotLight, MAX_NUMBER_OF_SPOT_LIGHTS);
}

GLuint LightsManager::AddLight(std::vector<Light*>& lightsVector, Light* light, int maxContainerSize)
{
	if (lightsVector.size() >= maxContainerSize) return Light::INITIALIZATION_ERROR;
	lightsVector.push_back(light);
	return (lightsVector.size() - 1);
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

void LightsManager::SetLightsInShader(const GLuint& shaderProgram)
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