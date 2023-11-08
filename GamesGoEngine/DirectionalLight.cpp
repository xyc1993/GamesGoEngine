#include "DirectionalLight.h"

#include "GameObject.h"
#include "RenderingManager.h"

namespace GamesGoEngine
{
	REGISTER_CLASS_STATIC(DirectionalLight);

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
		if (owner != nullptr)
		{
			direction = owner->GetTransform()->GetForward();
		}
	}

	void DirectionalLight::SetLightInShader(const unsigned int& shaderProgram, bool isNumberedLight, bool overrideThisLightNumber,
		int thisLightNumberOverride, bool overrideLightsNumber, int lightsNumberOverride)
	{
		const int lightNumber = overrideThisLightNumber ? thisLightNumberOverride : this->lightNumber;
		const std::string lightName = isNumberedLight ? GetNumberedShaderProperty(lightNumber) : "dirLight";

		glUseProgram(shaderProgram);
		if (overrideLightsNumber)
		{
			glUniform1i(glGetUniformLocation(shaderProgram, "dirLightsNumber"), lightsNumberOverride);
		}

		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".specular").c_str()), specular.x, specular.y, specular.z);

		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".direction").c_str()), direction.x, direction.y, direction.z);
	}

	std::string DirectionalLight::GetNumberedShaderProperty(int lightNumber)
	{
		std::string number = std::to_string(lightNumber);
		return ("dirLight[" + number + "]").c_str();
	}

	void DirectionalLight::InitMetaData()
	{
		metaData.className = CLASS_NAME(DirectionalLight);

		ADD_FIELD_META_DATA(metaData, glm::vec3, ambient, this->ambient)
		ADD_FIELD_META_DATA(metaData, glm::vec3, diffuse, this->diffuse)
		ADD_FIELD_META_DATA(metaData, glm::vec3, specular, this->specular)
	}
}