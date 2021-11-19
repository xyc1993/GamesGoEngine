#include "DirectionalLight.h"

#include "GameObject.h"
#include "LightsManager.h"

DirectionalLight::DirectionalLight()
{
	this->direction = glm::vec3(1.0f, -0.5f, 0.0f);

	this->lightNumber = LightsManager::AddDirectionalLight(this);
	if (this->lightNumber == INITIALIZATION_ERROR) delete this;
}

DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->direction = glm::vec3(1.0f, -0.5f, 0.0f);

	this->lightNumber = LightsManager::AddDirectionalLight(this);
	if (this->lightNumber == INITIALIZATION_ERROR) delete this;
}

void DirectionalLight::Update()
{
	Light::Update();

	if (owner != nullptr)
	{
		direction = owner->GetTransform()->GetForward();
	}
}

void DirectionalLight::SetLightInShader(const GLuint& shaderProgram)
{
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".specular").c_str()), specular.x, specular.y, specular.z);

	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".direction").c_str()), direction.x, direction.y, direction.z);
}

std::string DirectionalLight::GetNumberedShaderProperty()
{
	std::string number = std::to_string(lightNumber);
	return ("dirLight[" + number + "]").c_str();
}