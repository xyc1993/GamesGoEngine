#include "PointLight.h"

#include "GameObject.h"
#include "RenderingManager.h"

namespace GamesGoEngine
{
	REGISTER_CLASS_STATIC(PointLight);

	PointLight::PointLight()
	{
		this->lightNumber = RenderingManager::GetLightsManager()->AddPointLight(this);
		if (this->lightNumber == INITIALIZATION_ERROR) delete this;
	}

	PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float maxRadius, float maxRadiusFallOffStart)
	{
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;

		this->position = position;

		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;

		this->maxRadius = maxRadius;
		this->maxRadiusFallOffStart = maxRadiusFallOffStart;

		this->lightNumber = RenderingManager::GetLightsManager()->AddPointLight(this);
		if (this->lightNumber == INITIALIZATION_ERROR) delete this;
	}

	PointLight::~PointLight()
	{
		RenderingManager::GetLightsManager()->RemovePointLight(this);
	}

	void PointLight::Update()
	{
		if (owner != nullptr)
		{
			position = owner->GetTransform()->GetPosition();
		}
	}

	void PointLight::SetLightInShader(const unsigned int& shaderProgram, bool isNumberedLight,
		bool overrideThisLightNumber, int thisLightNumberOverride,
		bool overrideLightsNumber, int lightsNumberOverride)
	{
		const int lightNumber = overrideThisLightNumber ? thisLightNumberOverride : this->lightNumber;
		const std::string lightName = isNumberedLight ? GetNumberedShaderProperty(lightNumber) : "pointLight";

		glUseProgram(shaderProgram);
		if (overrideLightsNumber)
		{
			glUniform1i(glGetUniformLocation(shaderProgram, "pointLightsNumber"), lightsNumberOverride);
		}

		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".specular").c_str()), specular.x, specular.y, specular.z);

		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".position").c_str()), position.x, position.y, position.z);
		glUniform1f(glGetUniformLocation(shaderProgram, (lightName + ".constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(shaderProgram, (lightName + ".linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(shaderProgram, (lightName + ".quadratic").c_str()), quadratic);

		glUniform1f(glGetUniformLocation(shaderProgram, (lightName + ".maxRadius").c_str()), maxRadius);
		glUniform1f(glGetUniformLocation(shaderProgram, (lightName + ".maxRadiusFallOffStart").c_str()), maxRadiusFallOffStart);
	}

	std::string PointLight::GetNumberedShaderProperty(int lightNumber)
	{
		std::string number = std::to_string(lightNumber);
		return ("pointLights[" + number + "]").c_str();
	}

	void PointLight::InitMetaData()
	{
		metaData.className = CLASS_NAME(PointLight);

		ADD_FIELD_META_DATA(metaData, glm::vec3, ambient, this->ambient)
		ADD_FIELD_META_DATA(metaData, glm::vec3, diffuse, this->diffuse)
		ADD_FIELD_META_DATA(metaData, glm::vec3, specular, this->specular)

		ADD_FIELD_META_DATA(metaData, float, constant, this->constant)
		ADD_FIELD_META_DATA(metaData, float, linear, this->linear)
		ADD_FIELD_META_DATA(metaData, float, quadratic, this->quadratic)

		ADD_FIELD_META_DATA(metaData, float, maxRadius, this->maxRadius)
		ADD_FIELD_META_DATA(metaData, float, maxRadiusFallOffStart, this->maxRadiusFallOffStart)
	}
}