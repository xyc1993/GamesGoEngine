#include "DirectionalLight.h"

#include "GameObject.h"
#include "RenderingManager.h"

DirectionalLight::DirectionalLight()
{
	this->direction = glm::vec3(1.0f, -0.5f, 0.0f);

	this->lightNumber = RenderingManager::GetLightsManager()->AddDirectionalLight(this);
	if (this->lightNumber == INITIALIZATION_ERROR) delete this;
}

DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->direction = glm::vec3(1.0f, -0.5f, 0.0f);

	this->lightNumber = RenderingManager::GetLightsManager()->AddDirectionalLight(this);
	if (this->lightNumber == INITIALIZATION_ERROR) delete this;
}

DirectionalLight::~DirectionalLight()
{
	RenderingManager::GetLightsManager()->RemoveDirectionalLight(this);
}

void DirectionalLight::Update()
{
	Light::Update();

	if (owner != nullptr)
	{
		direction = owner->GetTransform()->GetForward();
	}
}

void DirectionalLight::SetThisLightInShader(const GLuint& shaderProgram)
{
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "dirLightsNumber"), 1);
	SetLightInShader(shaderProgram, 0);
}

void DirectionalLight::SetLightInShader(const GLuint& shaderProgram)
{
	SetLightInShader(shaderProgram, lightNumber);
}

void DirectionalLight::SetLightInShader(const GLuint& shaderProgram, int lightNumber)
{
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".specular").c_str()), specular.x, specular.y, specular.z);

	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".direction").c_str()), direction.x, direction.y, direction.z);
}

std::string DirectionalLight::GetNumberedShaderProperty(int lightNumber)
{
	std::string number = std::to_string(lightNumber);
	return ("dirLight[" + number + "]").c_str();
}
