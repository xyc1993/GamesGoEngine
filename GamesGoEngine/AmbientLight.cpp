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
	// In new iteration of light system this will be handled differently so currently ambient light doesn't get support for this
}

void AmbientLight::SetLightInShader(const GLuint& shaderProgram)
{
	// In new iteration of light system this will be handled differently so currently ambient light doesn't get support for this
}

std::string AmbientLight::GetNumberedShaderProperty(int lightNumber)
{
	return "";
}