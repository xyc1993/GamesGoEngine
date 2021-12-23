#include "LightsManager.h"

LightsManager* LightsManager::instance = new LightsManager();

LightsManager* LightsManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new LightsManager();
	}
	return instance;
}

GLuint LightsManager::AddDirectionalLight(DirectionalLight* directionalLight)
{
	return AddLight(GetInstance()->directionalLights, directionalLight, MAX_NUMBER_OF_DIR_LIGHTS);
}

GLuint LightsManager::AddPointLight(PointLight* pointLight)
{
	return AddLight(GetInstance()->pointLights, pointLight, MAX_NUMBER_OF_POINT_LIGHTS);
}

GLuint LightsManager::AddSpotLight(SpotLight* spotLight)
{
	return AddLight(GetInstance()->spotLights, spotLight, MAX_NUMBER_OF_SPOT_LIGHTS);
}

GLuint LightsManager::AddLight(std::vector<Light*>& lightsVector, Light* light, int maxContainerSize)
{
	if (lightsVector.size() >= maxContainerSize) return Light::INITIALIZATION_ERROR;
	lightsVector.push_back(light);
	return (lightsVector.size() - 1);
}

void LightsManager::RemoveDirectionalLight(DirectionalLight* directionalLight)
{
	RemoveLight(GetInstance()->directionalLights, directionalLight);
}

void LightsManager::RemovePointLight(PointLight* pointLight)
{
	RemoveLight(GetInstance()->pointLights, pointLight);
}

void LightsManager::RemoveSpotLight(SpotLight* spotLight)
{
	RemoveLight(GetInstance()->spotLights, spotLight);
}

void LightsManager::RemoveLight(std::vector<Light*>& lightsVector, Light* light)
{
	for (int i = 0; i < lightsVector.size(); i++)
	{
		if (lightsVector[i] == light)
		{
			lightsVector.erase(lightsVector.begin() + i);
		}
	}
}

void LightsManager::SetLightsInShader(const GLuint& shaderProgram)
{
	glUniform1i(glGetUniformLocation(shaderProgram, "dirLightsNumber"), GetInstance()->directionalLights.size());
	for (int i = 0; i < GetInstance()->directionalLights.size(); i++)
	{
		if (GetInstance()->directionalLights[i] != nullptr)
		{
			GetInstance()->directionalLights[i]->SetLightInShader(shaderProgram);
		}
	}	

	glUniform1i(glGetUniformLocation(shaderProgram, "pointLightsNumber"), GetInstance()->pointLights.size());
	for (int i = 0; i < GetInstance()->pointLights.size(); i++)
	{
		if (GetInstance()->pointLights[i] != nullptr)
		{
			GetInstance()->pointLights[i]->SetLightInShader(shaderProgram);
		}		
	}

	glUniform1i(glGetUniformLocation(shaderProgram, "spotLightsNumber"), GetInstance()->spotLights.size());
	for (int i = 0; i < GetInstance()->spotLights.size(); i++)
	{
		if (GetInstance()->spotLights[i] != nullptr)
		{
			GetInstance()->spotLights[i]->SetLightInShader(shaderProgram);
		}
	}
}