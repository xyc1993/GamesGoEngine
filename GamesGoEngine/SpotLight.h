#pragma once

#include "Light.h"

namespace GamesGoEngine
{
	class SpotLight : public Light
	{
	public:
		SpotLight();
		SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff);
		~SpotLight();

		void Update() override;
		void SetLightInShader(const GLuint& shaderProgram, bool isNumberedLight = true,
			bool overrideThisLightNumber = false, int thisLightNumberOverride = 0,
			bool overrideLightsNumber = false, int lightsNumberOverride = 0) override;

	protected:
		std::string GetNumberedShaderProperty(int lightNumber) override;
		virtual void InitMetaData() override;

	private:
		glm::vec3 position;
		glm::vec3 direction;

		float constant;
		float linear;
		float quadratic;

		float cutOff;
		float outerCutOff;
	};
}