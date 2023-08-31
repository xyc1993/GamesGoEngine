#include "AmbientLight.h"

#include "RenderingManager.h"

namespace GamesGoEngine
{
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
		RenderingManager::GetLightsManager()->RemoveAmbientLight(this);
	}

	void AmbientLight::CopyData(const AmbientLight& other)
	{
		this->ambient = other.ambient;
	}

	void AmbientLight::Update()
	{

	}

	void AmbientLight::SetLightInShader(const GLuint& shaderProgram, bool isNumberedLight, bool overrideThisLightNumber,
		int thisLightNumberOverride, bool overrideLightsNumber, int lightsNumberOverride)
	{
		/*
		 * Currently ambient light is implemented in a way that only 1 ambient light is supported so this method is much simpler than for other light classes
		 * Most likely more than 1 ambient light will never be needed as it's just ambient light to give some color to the darkest shadows
		 */

		glUseProgram(shaderProgram);
		glUniform1f(glGetUniformLocation(shaderProgram, "ambientLightActive"), 1.0f);
		glUniform3f(glGetUniformLocation(shaderProgram, "ambientLightColor"), ambient.x, ambient.y, ambient.z);
	}

	std::string AmbientLight::GetNumberedShaderProperty(int lightNumber)
	{
		/*
		 * Currently ambient light is implemented in a way that only 1 ambient light is supported so this method is much simpler than for other light classes
		 * Most likely more than 1 ambient light will never be needed as it's just ambient light to give some color to the darkest shadows
		 */

		return "";
	}
}