#include "SpotLight.h"

#include "GameObject.h"
#include "RenderingManager.h"

namespace GamesGoEngine
{
	REGISTER_CLASS_STATIC(SpotLight);

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
		if (owner != nullptr)
		{
			direction = owner->GetTransform()->GetForward();
			position = owner->GetTransform()->GetPosition();
		}
	}

	void SpotLight::SetLightInShader(const GLuint& shaderProgram, bool isNumberedLight, bool overrideThisLightNumber,
		int thisLightNumberOverride, bool overrideLightsNumber, int lightsNumberOverride)
	{
		const int lightNumber = overrideThisLightNumber ? thisLightNumberOverride : this->lightNumber;
		const std::string lightName = isNumberedLight ? GetNumberedShaderProperty(lightNumber) : "spotLight";

		glUseProgram(shaderProgram);
		if (overrideLightsNumber)
		{
			glUniform1i(glGetUniformLocation(shaderProgram, "spotLightsNumber"), lightsNumberOverride);
		}

		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".ambient").c_str()), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".specular").c_str()), specular.x, specular.y, specular.z);

		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".position").c_str()), position.x, position.y, position.z);
		glUniform3f(glGetUniformLocation(shaderProgram, (lightName + ".direction").c_str()), direction.x, direction.y, direction.z);

		glUniform1f(glGetUniformLocation(shaderProgram, (lightName + ".constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(shaderProgram, (lightName + ".linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(shaderProgram, (lightName + ".quadratic").c_str()), quadratic);

		glUniform1f(glGetUniformLocation(shaderProgram, (lightName + ".cutOff").c_str()), cutOff);
		glUniform1f(glGetUniformLocation(shaderProgram, (lightName + ".outerCutOff").c_str()), outerCutOff);
	}

	std::string SpotLight::GetNumberedShaderProperty(int lightNumber)
	{
		std::string number = std::to_string(lightNumber);
		return ("spotLight[" + number + "]").c_str();
	}

	void SpotLight::InitMetaData()
	{
		metaData.className = CLASS_NAME(SpotLight);

		ADD_FIELD_META_DATA(metaData, glm::vec3, ambient, this->ambient)
		ADD_FIELD_META_DATA(metaData, glm::vec3, diffuse, this->diffuse)
		ADD_FIELD_META_DATA(metaData, glm::vec3, specular, this->specular)

		ADD_FIELD_META_DATA(metaData, float, constant, this->constant)
		ADD_FIELD_META_DATA(metaData, float, linear, this->linear)
		ADD_FIELD_META_DATA(metaData, float, quadratic, this->quadratic)

		ADD_FIELD_META_DATA(metaData, float, cutOff, this->cutOff)
		ADD_FIELD_META_DATA(metaData, float, outerCutOff, this->outerCutOff)
	}
}