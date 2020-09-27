#include "LightsManager.h"

LightsManager::LightsManager(GLuint shaderProgram)
{
	this->shaderProgram = shaderProgram;

	this->directionalLightsNumber = 0;
	this->pointLightsNumber = 0;
	this->spotLightsNumber = 0;
}

void LightsManager::AddDirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction)
{
	if (directionalLightsNumber >= MAX_NUMBER_OF_DIR_LIGHTS) return;
	directionalLight[directionalLightsNumber] = DirectionalLight(shaderProgram, directionalLightsNumber, ambient, diffuse, specular, direction);
	directionalLightsNumber++;
}

void LightsManager::AddPointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
									glm::vec3 position, float constant, float linear, float quadratic)
{
	if (pointLightsNumber >= MAX_NUMBER_OF_POINT_LIGHTS) return;
	pointLights[pointLightsNumber] = PointLight(shaderProgram, pointLightsNumber, ambient, diffuse, specular, position, constant, linear, quadratic);
	pointLightsNumber++;
}

void LightsManager::AddSpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
									glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic,
									float cutOff, float outerCutOff)
{
	if (spotLightsNumber >= MAX_NUMBER_OF_SPOT_LIGHTS) return;
	spotLight[spotLightsNumber] = SpotLight(shaderProgram, spotLightsNumber, ambient, diffuse, specular, position, direction, constant, linear, quadratic, cutOff, outerCutOff);
	spotLightsNumber++;
}

void LightsManager::SetLightsInShader()
{
	glUniform1i(glGetUniformLocation(shaderProgram, "dirLightsNumber"), directionalLightsNumber);
	for (int i = 0; i < directionalLightsNumber; i++)
	{
		directionalLight[i].SetLightInShader();
	}	

	glUniform1i(glGetUniformLocation(shaderProgram, "pointLightsNumber"), pointLightsNumber);
	for (int i = 0; i < pointLightsNumber; i++)
	{
		pointLights[i].SetLightInShader();
	}

	glUniform1i(glGetUniformLocation(shaderProgram, "spotLightsNumber"), spotLightsNumber);
	for (int i = 0; i < spotLightsNumber; i++)
	{
		spotLight[i].SetLightInShader();
	}
}

DirectionalLight* LightsManager::GetDirectionalLight(int index)
{
	if (directionalLightsNumber == 0) return nullptr;
	if (index >= MAX_NUMBER_OF_DIR_LIGHTS) return nullptr;
	return &directionalLight[index];
}

PointLight* LightsManager::GetPointLight(int index)
{
	if (pointLightsNumber == 0) return nullptr;
	if (index >= MAX_NUMBER_OF_POINT_LIGHTS) return nullptr;
	return &pointLights[index];
}

SpotLight* LightsManager::GetSpotLight(int index)
{
	if (spotLightsNumber == 0) return nullptr;
	if (index >= MAX_NUMBER_OF_SPOT_LIGHTS) return nullptr;
	return &spotLight[index];
}