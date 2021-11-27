#include "LightsManager.h"

LightsManager* LightsManager::instance = new LightsManager();

LightsManager::LightsManager()
{
	this->directionalLightsNumber = 0;
	this->pointLightsNumber = 0;
	this->spotLightsNumber = 0;
}

LightsManager::~LightsManager()
{
	for (int i = 0; i < MAX_NUMBER_OF_DIR_LIGHTS; i++)
	{
		delete directionalLight[i];
	}
	for (int i = 0; i < MAX_NUMBER_OF_POINT_LIGHTS; i++)
	{
		delete pointLights[i];
	}
	for (int i = 0; i < MAX_NUMBER_OF_SPOT_LIGHTS; i++)
	{
		delete spotLight[i];
	}
}

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
	if (GetInstance()->directionalLightsNumber >= MAX_NUMBER_OF_DIR_LIGHTS) return Light::INITIALIZATION_ERROR;
	GetInstance()->directionalLight[GetInstance()->directionalLightsNumber] = directionalLight;

	const int lightIndex = GetInstance()->directionalLightsNumber;
	GetInstance()->directionalLightsNumber++;
	return lightIndex;
}

GLuint LightsManager::AddPointLight(PointLight* pointLight)
{
	if (GetInstance()->pointLightsNumber >= MAX_NUMBER_OF_POINT_LIGHTS) return Light::INITIALIZATION_ERROR;
	GetInstance()->pointLights[GetInstance()->pointLightsNumber] = pointLight;

	const int lightIndex = GetInstance()->pointLightsNumber;
	GetInstance()->pointLightsNumber++;
	return lightIndex;
}

GLuint LightsManager::AddSpotLight(SpotLight* spotLight)
{
	if (GetInstance()->spotLightsNumber >= MAX_NUMBER_OF_SPOT_LIGHTS) return Light::INITIALIZATION_ERROR;
	GetInstance()->spotLight[GetInstance()->spotLightsNumber] = spotLight;

	const int lightIndex = GetInstance()->spotLightsNumber;
	GetInstance()->spotLightsNumber++;
	return lightIndex;
}

void LightsManager::SetLightsInShader(const GLuint& shaderProgram)
{
	glUniform1i(glGetUniformLocation(shaderProgram, "dirLightsNumber"), GetInstance()->directionalLightsNumber);
	for (int i = 0; i < GetInstance()->directionalLightsNumber; i++)
	{
		GetInstance()->directionalLight[i]->SetLightInShader(shaderProgram);
	}	

	glUniform1i(glGetUniformLocation(shaderProgram, "pointLightsNumber"), GetInstance()->pointLightsNumber);
	for (int i = 0; i < GetInstance()->pointLightsNumber; i++)
	{
		GetInstance()->pointLights[i]->SetLightInShader(shaderProgram);
	}

	glUniform1i(glGetUniformLocation(shaderProgram, "spotLightsNumber"), GetInstance()->spotLightsNumber);
	for (int i = 0; i < GetInstance()->spotLightsNumber; i++)
	{
		GetInstance()->spotLight[i]->SetLightInShader(shaderProgram);
	}
}