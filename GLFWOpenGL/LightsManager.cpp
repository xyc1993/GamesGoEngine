#include "LightsManager.h"

LightsManager::LightsManager()
{
	this->directionalLightsNumber = 0;
	this->pointLightsNumber = 0;
	this->spotLightsNumber = 0;
}

void LightsManager::AddDirectionalLight(GLuint shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction)
{
	if (directionalLightsNumber >= 1) return;	
	directionalLight = DirectionalLight(shaderProgram, ambient, diffuse, specular, direction);
	directionalLightsNumber++;
}

void LightsManager::AddPointLight(GLuint shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
									glm::vec3 position, float constant, float linear, float quadratic)
{
	if (pointLightsNumber >= MAX_NUMBER_OF_POINT_LIGHTS) return;
	pointLights[pointLightsNumber] = PointLight(shaderProgram, ambient, diffuse, specular, pointLightsNumber, position, constant, linear, quadratic);
	pointLightsNumber++;
}

void LightsManager::AddSpotLight(GLuint shaderProgram, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
									glm::vec3 position, glm::vec3 direction, float constant, float linear, float quadratic,
									float cutOff, float outerCutOff)
{
	if (spotLightsNumber >= 1) return;	
	spotLight = SpotLight(shaderProgram, ambient, diffuse, specular, position, direction, constant, linear, quadratic, cutOff, outerCutOff);
	spotLightsNumber++;
}

void LightsManager::SetLightsInShader()
{
	//TO DO: after handling less than maximum number of lights in shader, adjust this method!

	directionalLight.SetLightInShader();

	for (int i = 0; i < MAX_NUMBER_OF_POINT_LIGHTS; i++)
	{
		pointLights[i].SetLightInShader();
	}

	spotLight.SetLightInShader();
}

DirectionalLight* LightsManager::GetDirectionalLight()
{
	if (directionalLightsNumber == 0) return nullptr;
	return &directionalLight;
}

PointLight* LightsManager::GetPointLight(int index)
{
	if (pointLightsNumber == 0) return nullptr;
	if (index >= MAX_NUMBER_OF_POINT_LIGHTS) return nullptr;
	return &pointLights[index];
}

SpotLight* LightsManager::GetSpotLight()
{
	if (spotLightsNumber == 0) return nullptr;
	return &spotLight;
}