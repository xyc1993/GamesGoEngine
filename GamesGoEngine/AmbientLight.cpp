#include "AmbientLight.h"

#include "RenderingManager.h"

AmbientLight::AmbientLight()
{
	this->ambient = glm::vec3(0.01f, 0.01f, 0.01f);

	this->lightNumber = RenderingManager::GetLightsManager()->AddAmbientLight(this);
	if (this->lightNumber == INITIALIZATION_ERROR) delete this;
}

AmbientLight::AmbientLight(glm::vec3 ambientColor)
{
	this->ambient = ambientColor;

	this->lightNumber = RenderingManager::GetLightsManager()->AddAmbientLight(this);
	if (this->lightNumber == INITIALIZATION_ERROR) delete this;
}

AmbientLight::~AmbientLight()
{
}

void AmbientLight::CopyData(const AmbientLight& other)
{
	this->ambient = other.ambient;
}

void AmbientLight::SetThisLightInShader(const GLuint& shaderProgram)
{
	glUseProgram(shaderProgram);
	glUniform1f(glGetUniformLocation(shaderProgram, "ambientLightActive"), 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "ambientLightColor"), ambient.x, ambient.y, ambient.z);	
}

void AmbientLight::SetLightInShader(const GLuint& shaderProgram)
{
	glUniform1f(glGetUniformLocation(shaderProgram, "ambientLightActive"), 1.0f);
	glUniform3f(glGetUniformLocation(shaderProgram, "ambientLightColor"), ambient.x, ambient.y, ambient.z);
}

std::string AmbientLight::GetNumberedShaderProperty(int lightNumber)
{
	return "";
}