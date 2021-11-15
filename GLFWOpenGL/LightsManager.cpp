#include "LightsManager.h"

LightsManager* LightsManager::instance = new LightsManager();

LightsManager::LightsManager()
{
	this->directionalLightsNumber = 0;
	this->pointLightsNumber = 0;
	this->spotLightsNumber = 0;
}

LightsManager* LightsManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new LightsManager();
	}
	return instance;
}

void LightsManager::AddDirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction)
{
	if (GetInstance()->directionalLightsNumber >= MAX_NUMBER_OF_DIR_LIGHTS) return;
	GetInstance()->directionalLight[GetInstance()->directionalLightsNumber] = DirectionalLight(GetInstance()->directionalLightsNumber, ambient, diffuse, specular, direction);
	GetInstance()->directionalLightsNumber++;
}

void LightsManager::AddPointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
									glm::vec3 position, float constant, float linear, float quadratic)
{
	if (GetInstance()->pointLightsNumber >= MAX_NUMBER_OF_POINT_LIGHTS) return;
	GetInstance()->pointLights[GetInstance()->pointLightsNumber] = PointLight(GetInstance()->pointLightsNumber, ambient, diffuse, specular, position, constant, linear, quadratic);
	GetInstance()->pointLightsNumber++;
}

void LightsManager::AddSpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
									glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic,
									float cutOff, float outerCutOff)
{
	if (GetInstance()->spotLightsNumber >= MAX_NUMBER_OF_SPOT_LIGHTS) return;
	GetInstance()->spotLight[GetInstance()->spotLightsNumber] = SpotLight(GetInstance()->spotLightsNumber, ambient, diffuse, specular, position, direction, constant, linear, quadratic, cutOff, outerCutOff);
	GetInstance()->spotLightsNumber++;
}

void LightsManager::SetLightsInShader(const GLuint& shaderProgram)
{
	glUniform1i(glGetUniformLocation(shaderProgram, "dirLightsNumber"), GetInstance()->directionalLightsNumber);
	for (int i = 0; i < GetInstance()->directionalLightsNumber; i++)
	{
		GetInstance()->directionalLight[i].SetLightInShader(shaderProgram);
	}	

	glUniform1i(glGetUniformLocation(shaderProgram, "pointLightsNumber"), GetInstance()->pointLightsNumber);
	for (int i = 0; i < GetInstance()->pointLightsNumber; i++)
	{
		GetInstance()->pointLights[i].SetLightInShader(shaderProgram);
	}

	glUniform1i(glGetUniformLocation(shaderProgram, "spotLightsNumber"), GetInstance()->spotLightsNumber);
	for (int i = 0; i < GetInstance()->spotLightsNumber; i++)
	{
		GetInstance()->spotLight[i].SetLightInShader(shaderProgram);
	}
}

DirectionalLight* LightsManager::GetDirectionalLight(int index)
{
	if (GetInstance()->directionalLightsNumber == 0) return nullptr;
	if (index >= MAX_NUMBER_OF_DIR_LIGHTS) return nullptr;
	return &GetInstance()->directionalLight[index];
}

PointLight* LightsManager::GetPointLight(int index)
{
	if (GetInstance()->pointLightsNumber == 0) return nullptr;
	if (index >= MAX_NUMBER_OF_POINT_LIGHTS) return nullptr;
	return &GetInstance()->pointLights[index];
}

SpotLight* LightsManager::GetSpotLight(int index)
{
	if (GetInstance()->spotLightsNumber == 0) return nullptr;
	if (index >= MAX_NUMBER_OF_SPOT_LIGHTS) return nullptr;
	return &GetInstance()->spotLight[index];
}