#include "SpotLight.h"

#include "GameObject.h"
#include "LightsManager.h"

SpotLight::SpotLight()
{
	this->lightNumber = LightsManager::AddSpotLight(this);
	if (this->lightNumber == INITIALIZATION_ERROR) delete this;
}

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->position = position;
	this->direction = direction;

	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;

	this->cutOff = cutOff;
	this->outerCutOff = outerCutOff;

	this->lightNumber = LightsManager::AddSpotLight(this);
	if (this->lightNumber == INITIALIZATION_ERROR) delete this;
}

SpotLight::~SpotLight()
{
	LightsManager::RemoveSpotLight(this);
}

void SpotLight::Update()
{
	Light::Update();

	if (owner != nullptr)
	{
		direction = owner->GetTransform()->GetForward();
		position = owner->GetTransform()->GetPosition();
	}
}

void SpotLight::SetLightInShader(const GLuint& shaderProgram)
{
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".specular").c_str()), specular.x, specular.y, specular.z);

	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".position").c_str()), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".direction").c_str()), direction.x, direction.y, direction.z);

	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".constant").c_str()), constant);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".quadratic").c_str()), quadratic);

	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".cutOff").c_str()), cutOff);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty() + ".outerCutOff").c_str()), outerCutOff);
}

std::string SpotLight::GetNumberedShaderProperty()
{
	std::string number = std::to_string(lightNumber);
	return ("spotLight[" + number + "]").c_str();
}