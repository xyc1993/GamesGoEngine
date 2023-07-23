#include "SpotLight.h"

#include "GameObject.h"
#include "RenderingManager.h"

SpotLight::SpotLight()
{
	this->lightNumber = RenderingManager::GetLightsManager()->AddSpotLight(this);
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

	this->lightNumber = RenderingManager::GetLightsManager()->AddSpotLight(this);
	if (this->lightNumber == INITIALIZATION_ERROR) delete this;
}

SpotLight::~SpotLight()
{
	RenderingManager::GetLightsManager()->RemoveSpotLight(this);
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

void SpotLight::SetThisLightInShader(const GLuint& shaderProgram)
{
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "spotLightsNumber"), 1);
	SetLightInShader(shaderProgram, 0);
}

void SpotLight::SetLightInShader(const GLuint& shaderProgram)
{
	SetLightInShader(shaderProgram, lightNumber);
}

void SpotLight::SetLightInShader(const GLuint& shaderProgram, int lightNumber)
{
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".specular").c_str()), specular.x, specular.y, specular.z);

	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".position").c_str()), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".direction").c_str()), direction.x, direction.y, direction.z);

	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".constant").c_str()), constant);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".quadratic").c_str()), quadratic);

	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".cutOff").c_str()), cutOff);
	glUniform1f(glGetUniformLocation(shaderProgram, (GetNumberedShaderProperty(lightNumber) + ".outerCutOff").c_str()), outerCutOff);
}

std::string SpotLight::GetNumberedShaderProperty(int lightNumber)
{
	std::string number = std::to_string(lightNumber);
	return ("spotLight[" + number + "]").c_str();
}
