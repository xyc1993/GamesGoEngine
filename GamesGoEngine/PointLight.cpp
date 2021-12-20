#include "PointLight.h"

#include "GameObject.h"
#include "LightsManager.h"

PointLight::PointLight()
{
	this->lightNumber = LightsManager::AddPointLight(this);
	if (this->lightNumber == INITIALIZATION_ERROR) delete this;
}

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->position = position;

	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;

	this->lightNumber = LightsManager::AddPointLight(this);
	if (this->lightNumber == INITIALIZATION_ERROR) delete this;
}

void PointLight::Update()
{
	Light::Update();

	if (owner != nullptr)
	{
		position = owner->GetTransform()->GetPosition();
	}
}

void PointLight::SetLightInShader(const GLuint& shaderProgram)
{
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".specular").c_str()), specular.x, specular.y, specular.z);

	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".position").c_str()), position.x, position.y, position.z);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".constant").c_str()), constant);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".quadratic").c_str()), quadratic);
}

std::string PointLight::GetNumberedShaderProperty()
{
	std::string number = std::to_string(lightNumber);
	return ("pointLights[" + number + "]").c_str();
}